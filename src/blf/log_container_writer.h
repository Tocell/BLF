#ifndef LOG_CONTAINER_H
#define LOG_CONTAINER_H

#include <valarray>
#include <iosfwd>

#include "blf_structure.h"
#include "zlib.h"

namespace BLF
{

class LogContainerWriter
{
public:
	LogContainerWriter();
	~LogContainerWriter() = default;

	void compress(uint16_t compression_method, int compression_level);

	void uncompress();

private:
	LogContainer log_container_;
};

inline LogContainerWriter::LogContainerWriter()
{
	log_container_.header_base.signature = BL_OBJ_SIGNATURE;
	log_container_.header_base.header_size = sizeof(ObjectHeaderBase);
	log_container_.header_base.header_version = 1;
	log_container_.header_base.object_size = sizeof(ObjectHeaderBase) + log_container_.compressed_file_size;
	log_container_.header_base.object_type = BL_OBJ_TYPE_LOG_CONTAINER;
}

inline void LogContainerWriter::compress(uint16_t compression_method, int compression_level)
{
	switch (compression_method)
	{
	case 0:
		memcpy(log_container_.compressed_file,
			log_container_.uncompressed_file,
			4 * 1024 * 1024);
		log_container_.compressed_file_size = log_container_.uncompressed_file_size;
		break;
	case 2:
		{
			uLong size = compressBound(log_container_.uncompressed_file_size);
			int ret = ::compress2(log_container_.compressed_file,
									&size,
									log_container_.uncompressed_file,
									log_container_.uncompressed_file_size,
									compression_level);
			if (ret != Z_OK)
			{
				throw std::runtime_error("compression failed");
			}
			log_container_.compressed_file_size = static_cast<uint32_t>(size);
			break;
		}
	default:
		break;
	}
}

inline void LogContainerWriter::uncompress()
{
	switch (log_container_.compression_method)
	{
	case 0:
		memcpy(log_container_.uncompressed_file,
			log_container_.compressed_file,
			4 * 1024 * 1024);
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

}

#endif //LOG_CONTAINER_H
