#include "blf_logger.h"
#include "can_message_blf_writer.h"

#include <iostream>

#include "can_message.h"
#include "blf_structure.h"

namespace BLF
{

BlfLogger::BlfLogger()
	: frame_count_(0),
	compression_method_(0),
	compression_level_(0)
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
	if (file_writer_.is_open())
	{
		std::cout << "BlfLogger::close file flush log container." << std::endl;
		flush_logcontainer(log_container_writer_.get_logcontainer());

		file_statistics_writer_.update_frame_count(frame_count_);
		file_statistics_writer_.update_file_size(file_writer_.tell());
		file_writer_.seek(0);
		file_statistics_writer_.update_file_header(file_writer_);
		file_writer_.close();
	}
}
	// std::map<BusType, std::unique_ptr<IMessageWriter>> writer_;
bool BlfLogger::write(const BusMessage& msg)
{
	const auto bus_type = msg.get_bus_type();
	auto it = writer_.find(bus_type);
	if (it == writer_.end())
	{
		return false;
	}

	bool result;
	if ((file_writer_.get_pos() + 400) >= BUFFER_MAX_SIZE)
	{
		flush_logcontainer(log_container_writer_.get_logcontainer());
	}

	result = it->second->write(msg, file_writer_);
	frame_count_++;

	return result;
}

void BlfLogger::flush_logcontainer(LogContainer& log_container)
{
	auto& lc = log_container_writer_.get_logcontainer();

	log_container_writer_.set_buffer(file_writer_.get_buffer(), file_writer_.get_pos());
	log_container_writer_.compress(compression_method_, compression_level_);

	LogContainerDiskHeader hdr{};
	hdr.base.signature      = BL_OBJ_SIGNATURE;
	hdr.base.header_size    = sizeof(ObjectHeaderBase);
	hdr.base.header_version = 1;
	hdr.base.object_type    = BL_OBJ_TYPE_LOG_CONTAINER;

	hdr.compressionMethod = lc.compression_method;
	hdr.reserved1         = 0;
	hdr.reserved2         = 0;
	hdr.uncompressedSize  = lc.uncompressed_file_size;
	hdr.reserved3         = 0;


	const uint32_t raw = lc.compressed_file_size;

	hdr.base.object_size = static_cast<uint32_t>(sizeof(LogContainerDiskHeader) + raw);

	file_writer_.write(reinterpret_cast<const uint8_t*>(&hdr), sizeof(hdr));
	file_writer_.write(lc.compressed_file, raw);

	const uint32_t pad = hdr.base.object_size % 4;
	if (pad)
	{
		static const uint8_t zeros[3] = {0,0,0};
		file_writer_.write(zeros, pad);
	}

	file_writer_.set_pos(0);
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

void BlfLogger::set_compres_level(int32_t compres_level)
{
	if (compres_level > compression_level_)
	{
		compression_method_ = 2;
	}
	else
	{
		compression_method_ = 0;
	}
	compression_level_ = compres_level;
	file_statistics_writer_.set_compres_level(compres_level);
}

void BlfLogger::set_timestamp_unit(int32_t unit)
{
	for (auto& it : writer_)
	{
		it.second->set_timestamp_unit(unit);
	}
}

}
