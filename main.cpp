#include <iostream>
#include "src/include/logger.h"
#include "src/include/types.h"
#include "src/include/can_object.h"
#include "src/object/can/can_message.h"
#include "src/object/can/message_factory.h"

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

	BLF::CanFrame can_frame{};
	can_frame.channel = 1;
	can_frame.flags = 1;
	can_frame.dlc = 8;
	can_frame.id = 0x123;
	for (auto i = 0; i < can_frame.dlc; i++)
	{
		can_frame.data[i] = i;
	}

	BLF::BusMessagePtr message(BLF::create_message(can_frame));
	logger->write(*message);

	logger->close();
	return 0;
}
