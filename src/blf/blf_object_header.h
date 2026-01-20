#ifndef BLF_OBJECT_HEADER_H
#define BLF_OBJECT_HEADER_H

#include <cstdint>
#include <ios>
#include "can_object.h"
#include "frame_type_define.h"

namespace GWLogger::Blf
{

#define BL_OBJ_SIGNATURE                 0x4A424F4C       /* object signature */
#define BL_FILE_SIGNATURE                0x47474F4C

#pragma pack(push, 1)
struct ObjectHeaderBase {
	uint32_t signature;        ///< Always 0x4A424F4C ('LOBJ')
	uint16_t header_size;      ///< Size of the complete header
	uint16_t header_version;   ///< Header version (1 or 2)  1：ObjectHeader  2：ObjectHeader2
	uint32_t object_size;      ///< Size of entire object including header
	uint32_t object_type;      ///< Type of object (see object_type_define.h)
};

struct ObjectHeader {
	uint32_t time_flags;     ///< Flags (e.g., 0x00000001 = 10microsecond timestamp)
	uint16_t client_index;     ///< Reserved
	uint16_t object_version;   ///< Object-specific version
	uint64_t object_timestamp; ///< Timestamp in nanoseconds
};

struct ObjectHeader2 {
	uint32_t object_flags;
	uint8_t timestamp_status;  ///< Timestamp status flags
	uint8_t reserved;
	uint16_t object_version;
	uint64_t object_timestamp;
	uint64_t original_timestamp; ///< Original timestamp before synchronization
	uint32_t object_flags2;    ///< Additional flags
	uint32_t reserved2;
	uint32_t reserved3;
};

struct SYSTEMTIME {
	uint16_t year;
	uint16_t month;
	uint16_t day_of_week;
	uint16_t day;
	uint16_t hour;
	uint16_t minute;
	uint16_t second;
	uint16_t milliseconds;
};

struct FileStatistics {
	uint32_t signature;
	uint32_t statistics_size;
	uint32_t api_number;
	uint8_t application_id;
	uint8_t compression_level;
	uint8_t application_major;
	uint8_t application_minor;
	uint64_t file_size;
	uint64_t uncompressed_file_size;
	uint32_t object_count;
	uint32_t application_build;
	SYSTEMTIME measurement_start_time;
	SYSTEMTIME last_object_time;
	uint64_t restore_points_offset;
	uint32_t reserved_file_statistics[16];
};

struct LogContainer
{
	ObjectHeaderBase header_base{};
	uint16_t compression_method {};
	uint16_t reserved_log_container1 {};
	uint32_t reserved_log_container2 {};
	uint32_t uncompressed_file_size {};
	uint32_t reserved_log_container3 {};
	uint8_t compressed_file[BUFFER_MAX_SIZE] {};
	uint8_t uncompressed_file[BUFFER_MAX_SIZE] {};
	uint32_t compressed_file_size {};
	std::streampos file_position {};
};

struct LogContainerDiskHeader
{
	ObjectHeaderBase base;
	uint16_t compressionMethod;
	uint16_t reserved1;
	uint32_t reserved2;
	uint32_t uncompressedSize;
	uint32_t reserved3;
};

#pragma pack(pop)

static_assert(sizeof(ObjectHeaderBase) == 16, "ObjectHeaderBase must be 16 bytes");
static_assert(sizeof(ObjectHeader)     == 16, "ObjectHeader must be 16 bytes");
static_assert(sizeof(LogContainerDiskHeader) == 32, "LogContainer header must be 32 bytes");
static_assert(sizeof(ObjectHeaderBase) + sizeof(ObjectHeader) + sizeof(CanFrame) == 48);
static_assert(sizeof(FileStatistics) == 144, "FileStatistics must be 144 bytes");

}
#endif //BLF_OBJECT_HEADER_H
