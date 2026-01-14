#include "log_container_handler.h"
#include "zlib.h"
#include <valarray>
#include <iosfwd>
#include <iostream>

namespace BLF
{

LogContainerHandler::LogContainerHandler()
{
	log_container_.header_base.signature		= BL_OBJ_SIGNATURE;
	log_container_.header_base.header_size		= sizeof(ObjectHeaderBase);
	log_container_.header_base.header_version	= 1;
	log_container_.header_base.object_type		= BL_OBJ_TYPE_LOG_CONTAINER;

	log_container_.header_base.object_size		= 0;
}

size_t LogContainerHandler::calculate_size()
{
	return sizeof(ObjectHeaderBase) +
		sizeof(uint16_t) + sizeof(uint16_t) +
		sizeof(uint32_t) + sizeof(uint32_t) +
		sizeof(uint32_t);
}

void LogContainerHandler::compress(uint16_t compression_method, int compression_level)
{
	log_container_.compression_method = compression_method;
	switch (compression_method)
	{
	case 0:
		memcpy(log_container_.compressed_file,
			log_container_.uncompressed_file,
			log_container_.uncompressed_file_size);
		log_container_.compressed_file_size = log_container_.uncompressed_file_size;
		log_container_.header_base.object_size = calculate_size() + log_container_.compressed_file_size;

		break;
	case 2:
		{
			uLong compress_size = compressBound(log_container_.uncompressed_file_size);
			int ret = ::compress2(
									reinterpret_cast<Byte*>(log_container_.compressed_file),
									&compress_size,
									reinterpret_cast<Byte*>(log_container_.uncompressed_file),
									log_container_.uncompressed_file_size,
									compression_level);
			if (ret != Z_OK)
			{
				throw std::runtime_error("compression failed");
			}
			log_container_.compressed_file_size = static_cast<uint32_t>(compress_size);

			log_container_.header_base.object_size = calculate_size() +
				log_container_.compressed_file_size;
			break;
		}
	default:
		break;
	}
}

void LogContainerHandler::uncompress()
{
	switch (log_container_.compression_method)
	{
	case 0:
		memcpy(log_container_.uncompressed_file,
			log_container_.compressed_file,
			log_container_.uncompressed_file_size);
		break;
	case 2:
		{
			uLong size = log_container_.uncompressed_file_size;
			int ret = ::uncompress(log_container_.uncompressed_file,
									&size,
									log_container_.compressed_file,
									log_container_.compressed_file_size);
			if (ret != Z_OK || size != static_cast<uLong>(log_container_.uncompressed_file_size))
			{
				throw std::runtime_error("uncompression failed");
			}
			break;
		}
	default:
		throw std::runtime_error("uncompression failed");
	}
}

LogContainer& LogContainerHandler::get_logcontainer()
{
	return log_container_;
}

void LogContainerHandler::set_buffer(uint8_t* data, size_t size)
{
	if (size > BUFFER_MAX_SIZE) {
		throw std::runtime_error("set_buffer: size exceeds BUFFER_MAX_SIZE");
	}
	log_container_.uncompressed_file_size = size;
	memcpy(log_container_.uncompressed_file, data, size);
}

void LogContainerHandler::reset_logcontainer()
{

}

}