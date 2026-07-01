#include "gw_logger.h"

#include <benchmark/benchmark.h>

#include <atomic>
#include <chrono>
#include <cstdio>
#include <memory>
#include <string>
#include <thread>

#define OBJECT_SIZE 100000000

namespace
{

uint64_t posix_time_ns_uint64()
{
	const auto now = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(
		now.time_since_epoch()).count();
}

double elapsed_seconds(std::chrono::steady_clock::time_point begin)
{
	return std::chrono::duration<double>(std::chrono::steady_clock::now() - begin).count();
}

std::unique_ptr<GWLogger::Logger> open_blf_writer(const std::string& path)
{
	auto logger = GWLogger::Logger::create(GWLogger::FileFormat::BLF);
	if (!logger || !logger->open(path, GWLogger::OpenMode::Write) || !logger->is_open())
	{
		return nullptr;
	}

	logger->set_compres_level(2);
	logger->set_timestamp_unit(GWLogger::TimeStampUnit::BLF_TIME_ONE_NANS);
	return logger;
}

GWLogger::CanFrame make_can_frame(int i)
{
	GWLogger::CanFrame frame{};
	frame.channel = 1;
	frame.flags = GWLogger::TX;
	frame.dlc = 8;
	frame.id = static_cast<uint32_t>(0x100 + (i % 0x700));
	for (uint8_t j = 0; j < frame.dlc; ++j)
	{
		frame.data[j] = static_cast<uint8_t>(i + j);
	}
	return frame;
}

GWLogger::CanFdFrame make_canfd_frame(int i)
{
	GWLogger::CanFdFrame frame{};
	frame.channel = 2;
	frame.flags = GWLogger::TX;
	frame.dlc = 15;
	frame.id = static_cast<uint32_t>(0x200 + (i % 0x700));
	frame.frame_length = 0;
	frame.arb_bit_count = 0;
	frame.can_fd_flags = GWLogger::EDL | GWLogger::BRS;
	frame.valid_data_bytes = 64;
	for (uint8_t j = 0; j < frame.valid_data_bytes; ++j)
	{
		frame.data[j] = static_cast<uint8_t>(i + j + 1);
	}
	return frame;
}

GWLogger::CanFdFrame64 make_canfd64_frame(int i)
{
	GWLogger::CanFdFrame64 frame{};
	frame.channel = 1;
	frame.dlc = 15;
	frame.valid_data_bytes = 64;
	frame.tx_count = 1;
	frame.id = static_cast<uint32_t>(0x300 + (i % 0x700));
	frame.frame_length = 0;
	frame.flags = 0x1000;
	frame.btr_cfg_arb = 0;
	frame.btr_cfg_data = 0;
	frame.time_offset_brs_ns = 0;
	frame.time_offset_crc_del_ns = 0;
	frame.bit_count = 0;
	frame.dir = 1;
	frame.ext_data_offset = 0;
	frame.crc = 0;
	for (uint8_t j = 0; j < frame.valid_data_bytes; ++j)
	{
		frame.data[j] = static_cast<uint8_t>(i + j + 2);
	}
	return frame;
}

GWLogger::FlexRayFrame make_flexray_frame(int i)
{
	GWLogger::FlexRayFrame frame{};
	frame.channel = 1;
	frame.dir = 0;
	frame.low_time = 0;
	frame.fpga_tick = 0;
	frame.fpga_tick_overflow = 0;
	frame.client_index = 0;
	frame.cluster_time = 0;
	frame.frame_id = static_cast<uint16_t>(1 + (i % 2047));
	frame.header_crc = 0;
	frame.frame_state = 0;
	frame.length = 32;
	frame.cycle = static_cast<uint8_t>(i % 64);
	frame.header_bit_mask = 0;
	frame.reserved1 = 0;
	frame.reserved2 = 0;
	for (uint8_t j = 0; j < frame.length; ++j)
	{
		frame.data[j] = static_cast<uint8_t>(i + j + 3);
	}
	return frame;
}

GWLogger::LinFrame make_lin_frame(int i)
{
	GWLogger::LinFrame frame{};
	frame.channel = 1;
	frame.id = static_cast<uint8_t>(0x10 + (i % 0x30));
	frame.dlc = 8;
	for (uint8_t j = 0; j < frame.dlc; ++j)
	{
		frame.data[j] = static_cast<uint8_t>(i + j + 4);
	}
	frame.fsm_id = 1;
	frame.fsm_state = 2;
	frame.header_time = 100;
	frame.full_time = 105;
	frame.crc = 0;
	frame.dir = 0;
	frame.reserved = 0;
	return frame;
}

template <typename FrameT>
void write_frame(GWLogger::Logger& logger, const FrameT& frame, uint64_t timestamp_ns)
{
	auto message = GWLogger::make_message(frame);
	message->set_timestamp(timestamp_ns);
	benchmark::DoNotOptimize(message.get());
	logger.write(std::move(message));
}

} // namespace

static void BM_BLFWrite_CorrectLifecycle(benchmark::State& state)
{
	constexpr int64_t kFrameTypesPerLoop = 5;
	constexpr int64_t kTotalObjectCount = OBJECT_SIZE * kFrameTypesPerLoop;
	const int64_t bytes_per_loop = OBJECT_SIZE * (
		static_cast<int64_t>(sizeof(GWLogger::CanFrame)) +
		static_cast<int64_t>(sizeof(GWLogger::CanFdFrame)) +
		static_cast<int64_t>(sizeof(GWLogger::CanFdFrame64)) +
		static_cast<int64_t>(sizeof(GWLogger::FlexRayFrame)) +
		static_cast<int64_t>(sizeof(GWLogger::LinFrame)));

	for (auto _ : state)
	{
		auto logger = open_blf_writer("benchmark_write_final.blf");
		if (!logger)
		{
			state.SkipWithError("failed to open benchmark_write_final.blf");
			return;
		}

		std::atomic<int64_t> object_count{0};
		std::atomic<bool> progress_done{false};
		const auto progress_begin = std::chrono::steady_clock::now();

		std::thread progress_thread([&]() {
			while (!progress_done.load())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));

				const auto done = object_count.load();
				const auto elapsed = elapsed_seconds(progress_begin);
				if (done <= 0 || elapsed <= 0.0)
				{
					continue;
				}

				const auto percent = 100.0 * static_cast<double>(done) /
					static_cast<double>(kTotalObjectCount);
				const auto fps = static_cast<double>(done) / elapsed;
				const auto remain = fps > 0.0
					? static_cast<double>(kTotalObjectCount - done) / fps
					: 0.0;

				std::printf(
					"progress : %lld/%lld %.2f%% elapsed=%.3fs remain=%.3fs speed=%.0f frame/s\n",
					static_cast<long long>(done),
					static_cast<long long>(kTotalObjectCount),
					percent,
					elapsed,
					remain,
					fps);
			}
		});

		const auto base_time = posix_time_ns_uint64();
		for (int i = 0; i < OBJECT_SIZE; ++i)
		{
			const auto timestamp = base_time + static_cast<uint64_t>(i * 5);
			write_frame(*logger, make_can_frame(i), timestamp + 0);
			object_count.fetch_add(1);
			write_frame(*logger, make_canfd_frame(i), timestamp + 1);
			object_count.fetch_add(1);
			write_frame(*logger, make_canfd64_frame(i), timestamp + 2);
			object_count.fetch_add(1);
			write_frame(*logger, make_flexray_frame(i), timestamp + 3);
			object_count.fetch_add(1);
			write_frame(*logger, make_lin_frame(i), timestamp + 4);
			object_count.fetch_add(1);
		}

		logger->close();

		progress_done.store(true);
		if (progress_thread.joinable())
		{
			progress_thread.join();
		}

		const auto end_time = posix_time_ns_uint64();
		std::printf(
			"end time : %llu object_count = %lld\n",
			static_cast<unsigned long long>(end_time),
			static_cast<long long>(object_count.load()));
	}

	state.SetBytesProcessed(state.iterations() * bytes_per_loop);
}

BENCHMARK(BM_BLFWrite_CorrectLifecycle);
