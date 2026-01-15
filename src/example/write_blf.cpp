#include <iostream>
#include "../include/logger.h"
#include "../include/types.h"
#include "../include/can_object.h"
#include "../object/can/can_message.h"
#include "../include/message_factory.h"

#include <chrono>
#include <thread>

inline uint64_t posix_time_us_uint64()
{
	auto now = std::chrono::system_clock::now();
	return std::chrono::time_point_cast<std::chrono::microseconds>(now).time_since_epoch().count();
}

int main()
{
	auto logger = BLF::Logger::create(BLF::FileFormat::BLF);
	logger->open("test.blf", BLF::OpenMode::Write);
	if (logger && logger->is_open())
	{
		std::cout << "file test.blf open successful." << std::endl;
	}
	else
	{
		std::cout << "file test.blf open failed." << std::endl;
		return -1;
	}

	logger->set_compres_level(6);

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

	for (int i = 0; i < 1000000; i++)
	{
		next += period;

		BLF::CanFrame can_frame{};
		can_frame.channel = 1;
		can_frame.flags = 1;
		can_frame.dlc = 8;
		can_frame.id = 0x123;
		for (auto j = 0; j < can_frame.dlc; j++)
		{
			can_frame.data[j] = (j + 1) * i;
		}
		auto message = make_message(can_frame);

		auto time = posix_time_us_uint64();
		message->set_timestamp(time * 1000ULL);

		logger->write(std::move(message));
		write_cnt.fetch_add(1);

		// std::this_thread::sleep_until(next);
	}

	is_running.store(false);
	if (tid.joinable())
		tid.join();
	logger->close();
	return 0;
}
