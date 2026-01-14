#include "asc_logger.h"
#include "can_message_asc_writer.h"
#include "../registry/writer_registry.h"

namespace BLF
{

AscLogger::AscLogger()
{
	writer_ = WriterRegistry::get_instance().create_writers(FileFormat::ASC);
}

bool AscLogger::open(const std::string& filepath, int32_t mode, bool append)
{
	file_writer_.open(filepath, mode, append);
	return true;
}

void AscLogger::close()
{

}

bool AscLogger::is_open() const
{
	return true;
}

uint64_t AscLogger::get_message_count() const
{
	return 0;
}

uint64_t AscLogger::get_file_size() const
{
	return 0;
}

bool AscLogger::write(BusMessagePtr msg)
{
	// const auto bus_type = msg.get_bus_type();
	// auto it = writer_.find(bus_type);
	// if (it == writer_.end())
	// {
	// 	return false;
	// }
	// return it->second->write(msg, file_writer_);
	return true;
}

void AscLogger::register_writer()
{
	writer_[BusType::CAN] = std::make_unique<CanMessageAscWriter>();
}

void AscLogger::set_compres_level(int32_t compres_level)
{

}

void AscLogger::set_timestamp_unit(int32_t unit)
{

}

}
