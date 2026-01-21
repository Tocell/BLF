#include <iostream>
#include "logger.h"
#include "bus_message.h"
#include "can_object.h"

#include "message_factory.h"

#include "can_message.h"
#include "canfd_message.h"

#include <chrono>
#include <thread>

inline uint64_t posix_time_us_uint64()
{
	auto now = std::chrono::system_clock::now();
	return std::chrono::time_point_cast<std::chrono::microseconds>(now).time_since_epoch().count();
}

int main()
{
	auto logger = GWLogger::Logger::create(GWLogger::FileFormat::ASC);
	logger->open("test.asc", GWLogger::OpenMode::Write);
	if (logger && logger->is_open())
	{
		std::cout << "file test.asc open successful." << std::endl;
	}
	else
	{
		std::cout << "file test.asc open failed." << std::endl;
		return -1;
	}

	auto next = std::chrono::steady_clock::now();
	constexpr auto period = std::chrono::microseconds(10);

	std::atomic<bool> is_running{true};
	std::atomic<int32_t> write_cnt{0};
	std::thread tid = std::thread([&is_running, &logger, &write_cnt]()
	{
		auto next = std::chrono::steady_clock::now();
		constexpr auto period = std::chrono::seconds(2);
		while (is_running)
		{
			next += period;
			printf("Write Frame %d Queue Count : %llu \n", write_cnt.load(), logger->get_message_count());
			std::this_thread::sleep_until(next);
		}
	});

	uint32_t id = 0x123;
	for (int i = 0; i < 100; i++)
	{
		next += period;

		GWLogger::CanFrame can_frame{};
		can_frame.channel = 1;
		can_frame.flags = 1;
		can_frame.dlc = 8;
		can_frame.id = (++id) % 2047;
		for (auto j = 0; j < can_frame.dlc; j++)
		{
			can_frame.data[j] = ((j + 1) * i) % 200;
		}
		auto message = make_message(can_frame);

		auto time = posix_time_us_uint64();
		message->set_timestamp(time);

		logger->write(std::move(message));
		write_cnt.fetch_add(1);

		// std::this_thread::sleep_until(next);
	}

	id = 0x125;
	write_cnt = 0;
	for (int i = 0; i < 100; i++)
	{
		next += period;

		GWLogger::CanFdFrame can_frame{};
		can_frame.channel = 1;
		can_frame.flags = 1;
		can_frame.dlc = 8;
		can_frame.id = (++id) % 2047;

		can_frame.valid_data_bytes = 8;
		can_frame.can_fd_flags     = 1;
		can_frame.frame_length     = 8;
		can_frame.arb_bit_count    = 0;

		for (auto j = 0; j < can_frame.dlc; j++)
		{
			can_frame.data[j] = ((j + 1) * i) % 200;
		}
		auto message = make_message(can_frame);

		auto time = posix_time_us_uint64();
		message->set_timestamp(time);

		logger->write(std::move(message));
		write_cnt.fetch_add(1);
	}
	std::cout << "write CANFD  " << write_cnt << " frame." << std::endl;

	is_running.store(false);
	if (tid.joinable())
		tid.join();
	logger->close();
	return 0;
}
