#include <iostream>
#include "src/include/logger.h"
#include "src/include/types.h"
#include "src/include/can_object.h"
#include "src/object/can/can_message.h"
#include "src/object/can/message_factory.h"

#include <chrono>

inline uint64_t posix_time_us_uint64()
{
	auto now = std::chrono::system_clock::now();
	return std::chrono::time_point_cast<std::chrono::microseconds>(now).time_since_epoch().count();
}

int main()
{
	auto logger = BLF::Logger::create(BLF::FileFormat::BLF);
	logger->open("test.blf", 0, true);
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

	BLF::CanFrame can_frame{};
	can_frame.channel = 1;
	can_frame.flags = 0;
	can_frame.dlc = 8;
	can_frame.id = 0x123;
	for (auto i = 0; i < can_frame.dlc; i++)
	{
		can_frame.data[i] = i;
	}

	BLF::BusMessagePtr message(BLF::create_message(can_frame));

	for (int i = 0; i < 1000; i++)
	{
		// std::cout << "write : " << i << std::endl;
		message->set_timestamp(posix_time_us_uint64());
		logger->write(*message);
	}

	logger->close();
	return 0;
}
