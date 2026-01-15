#include <iostream>
#include "../include/logger.h"
#include "../include/types.h"
#include "../include/can_object.h"
#include "../object/can/can_message.h"
#include "../include/message_factory.h"

#include <chrono>
#include <thread>

int main()
{
	auto logger = BLF::Logger::create(BLF::FileFormat::BLF);
	logger->open("test.blf", BLF::OpenMode::Read);
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

	logger->close();
	return 0;
}
