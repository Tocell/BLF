#include <iostream>
#include "logger.h"
#include "bus_message.h"
#include "can_object.h"
#include "can_message.h"
#include "can_message2.h"
#include "canfd_message.h"
#include "message_factory.h"

#include <chrono>
#include <thread>

int main()
{
	auto logger = GWLogger::Logger::create(GWLogger::FileFormat::BLF);
	logger->open("test.blf", GWLogger::OpenMode::Read);
	if (logger && logger->is_open())
	{
		std::cout << "file test.blf open successful." << std::endl;
	}
	else
	{
		std::cout << "file test.blf open failed." << std::endl;
		return -1;
	}

	uint64_t start_time, stop_time;
	logger->get_measure_time(start_time, stop_time);
	printf("File measure start_time: %llu  stop time: %llu\n", start_time, stop_time);

	int32_t cnt = 0;
	GWLogger::BusMessagePtr msg{};
	while (true)
	{
		if (!logger->read(msg))
		{
			break;
		}

		const auto type = msg->get_bus_type();
		const auto ts   = msg->get_timestamp();
		if (type == GWLogger::BusType::CAN)
		{
			auto* can = dynamic_cast<GWLogger::CanMessage*>(msg.get());
			if (!can) continue;

			const GWLogger::CanFrame& f = can->get_frame();
			printf("[CAN] %d ts=%llu ch=%u id=0x%X dlc=%u data=",
				   ++cnt, (unsigned long long)ts, f.channel, f.id, f.dlc);
			for (int i = 0; i < f.dlc && i < 8; ++i)
				printf("%02X ", f.data[i]);
			printf("\n");
		}
		else if (type == GWLogger::BusType::CAN2)
		{
			auto* can = dynamic_cast<GWLogger::CanMessage2*>(msg.get());
			if (!can) continue;

			const GWLogger::CanFrame2& f = can->get_frame();
			printf("[CAN2] %d ts=%llu ch=%u id=0x%X dlc=%u data=",
				   ++cnt, (unsigned long long)ts, f.channel, f.id, f.dlc);
			for (int i = 0; i < f.dlc && i < 8; ++i)
				printf("%02X ", f.data[i]);
			printf("\n");
		}
		else if (type == GWLogger::BusType::CAN_FD)
		{
			auto* can = dynamic_cast<GWLogger::CanFdMessage*>(msg.get());
			if (!can) continue;

			const GWLogger::CanFdFrame& f = can->get_frame();
			printf("[CANFD] %d ts=%llu ch=%u id=0x%X dlc=%u data=",
				   ++cnt, (unsigned long long)ts, f.channel, f.id, f.dlc);
			for (int i = 0; i < f.dlc && i < 8; ++i)
				printf("%02X ", f.data[i]);
			printf("\n");
		}
	}

	logger->close();
	return 0;
}
