#include "gw_logger.h"

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

namespace
{

void print_usage(const char* exe)
{
	std::cout << "Usage: " << exe << " [input.blf] [output.asc]\n"
		<< "Default: test.blf converted.asc\n";
}

uint64_t blf_timestamp_delta_us(uint64_t timestamp, uint64_t start_time_us, uint64_t stop_time_us)
{
	const uint64_t duration_us = stop_time_us > start_time_us
		? stop_time_us - start_time_us
		: 0ULL;
	constexpr uint64_t kToleranceUs = 1000000ULL;

	const auto in_measure_range = [&](uint64_t value_us) {
		if (start_time_us == 0 || duration_us == 0)
		{
			return false;
		}
		return value_us >= start_time_us &&
			value_us <= stop_time_us + kToleranceUs;
	};

	const uint64_t timestamp_as_us = timestamp / 1000ULL;
	if (in_measure_range(timestamp_as_us))
	{
		return timestamp_as_us - start_time_us;
	}

	if (in_measure_range(timestamp))
	{
		return timestamp - start_time_us;
	}

	if (duration_us > 0)
	{
		if (timestamp <= duration_us + kToleranceUs)
		{
			return timestamp;
		}
		if (timestamp_as_us <= duration_us + kToleranceUs)
		{
			return timestamp_as_us;
		}
	}

	return timestamp_as_us;
}

} // namespace

int main(int argc, char* argv[])
{
	if (argc > 3)
	{
		print_usage(argv[0]);
		return 1;
	}

	const std::string input_path = argc >= 2 ? argv[1] : "GEN6_0406_260716_183406_can.blf";
	const std::string output_path = argc >= 3 ? argv[2] : "GEN6_0406_260716_183406_can.asc";

	auto blf_reader = GWLogger::Logger::create(GWLogger::FileFormat::BLF);
	if (!blf_reader || !blf_reader->open(input_path, GWLogger::OpenMode::Read) || !blf_reader->is_open())
	{
		std::cerr << "open BLF failed: " << input_path << '\n';
		return 1;
	}

	auto asc_writer = GWLogger::Logger::create(GWLogger::FileFormat::ASC);
	if (!asc_writer || !asc_writer->open(output_path, GWLogger::OpenMode::Write) || !asc_writer->is_open())
	{
		std::cerr << "open ASC failed: " << output_path << '\n';
		blf_reader->close();
		return 1;
	}

	uint64_t start_time = 0;
	uint64_t stop_time = 0;
	blf_reader->get_measure_time(start_time, stop_time);

	uint64_t asc_start_time = 0;
	uint64_t asc_stop_time = 0;
	asc_writer->get_measure_time(asc_start_time, asc_stop_time);

	std::cout << "BLF input : " << input_path << '\n'
		<< "ASC output: " << output_path << '\n'
		<< "Measure  : " << start_time << " -> " << stop_time << '\n';

	const auto begin = std::chrono::steady_clock::now();
	uint64_t read_count = 0;
	uint64_t queued_count = 0;
	uint64_t first_delta_us = 0;
	uint64_t last_delta_us = 0;

	GWLogger::BusMessagePtr message;
	while (blf_reader->read(message))
	{
		if (!message)
		{
			continue;
		}
		++read_count;

		const uint64_t raw_timestamp = message->get_timestamp();
		const uint64_t delta_us = blf_timestamp_delta_us(raw_timestamp, start_time, stop_time);
		if (read_count == 1)
		{
			first_delta_us = delta_us;
		}
		last_delta_us = delta_us;
		message->set_timestamp(asc_start_time + delta_us);

		if (asc_writer->write(std::move(message)))
		{
			++queued_count;
		}
	}

	blf_reader->close();
	asc_writer->close();

	const auto elapsed = std::chrono::duration<double>(
		std::chrono::steady_clock::now() - begin).count();

	std::cout << "Read frames     : " << read_count << '\n'
		<< "Queued frames   : " << queued_count << '\n'
		<< "ASC time span   : " << first_delta_us << " -> " << last_delta_us << " us\n"
		<< "Elapsed seconds : " << elapsed << '\n';

	return 0;
}
