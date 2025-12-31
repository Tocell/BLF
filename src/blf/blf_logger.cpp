#include "blf_logger.h"
#include "can_message_blf_writer.h"

#include <iostream>

namespace BLF
{

bool BlfLogger::open(const std::string& filepath, bool append)
{
	return true;
}

void BlfLogger::close()
{

}

void BlfLogger::register_writer()
{
	writer_[BusType::CAN] = std::make_unique<CanMessageWriter>();
}

bool BlfLogger::write(const BusMessage& msg)
{
	return true;
}

bool BlfLogger::is_open() const
{
	return true;
}

uint64_t BlfLogger::get_message_count() const
{
	return 0;
}

uint64_t BlfLogger::get_file_size() const
{
	return 0;
}

void BlfLogger::flush()
{

}

}