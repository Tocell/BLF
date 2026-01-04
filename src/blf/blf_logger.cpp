#include "blf_logger.h"
#include "can_message_blf_writer.h"

#include <iostream>

namespace BLF
{

BlfLogger::BlfLogger()
{
	writer_ = WriterRegistry::get_instance().create_writers(FileFormat::BLF);
}

bool BlfLogger::open(const std::string& filepath, bool append)
{
	file_writer_.open(filepath, append);
	return true;
}

void BlfLogger::close()
{
	file_writer_.close();
}

bool BlfLogger::write(const BusMessage& msg)
{
	const auto bus_type = msg.get_bus_type();
	auto it = writer_.find(bus_type);
	if (it == writer_.end())
	{
		return false;
	}
	return it->second->write(msg, file_writer_);
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