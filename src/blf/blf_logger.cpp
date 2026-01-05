#include "blf_logger.h"
#include "can_message_blf_writer.h"

#include <iostream>

#include "can_message.h"

namespace BLF
{

BlfLogger::BlfLogger()
	: frame_count_(0)
{
	writer_ = WriterRegistry::get_instance().create_writers(FileFormat::BLF);
}

bool BlfLogger::open(const std::string& filepath, int32_t mode, bool append)
{
	file_writer_.open(filepath, mode, append);
	file_statistics_writer_.write_file_header(file_writer_);
	return true;
}

void BlfLogger::close()
{
	file_statistics_writer_.update_frame_count(frame_count_);
	file_statistics_writer_.update_file_size(file_writer_.tell());

	file_writer_.seek(0);
	file_statistics_writer_.update_file_header(file_writer_);
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

void BlfLogger::set_compres_level(int32_t compres_level)
{
	file_statistics_writer_.set_compres_level(compres_level);
}

}
