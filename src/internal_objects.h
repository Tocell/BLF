#ifndef INTERNAL_OBJECT_H
#define INTERNAL_OBJECT_H

#include "include/blf_objects.h"
#include <cstdint>
#pragma pack(push, 1)

struct FileStatistics
{
	uint32_t signature = 0x47474F4C;
	uint32_t statistics_size = 0;
	uint32_t api_number = 4080200;
	uint8_t application_id = 0;
	uint8_t compression_level = 0;
	uint8_t application_major = 0;
	uint8_t application_minor = 0;
	uint64_t file_size = 0;
	uint64_t uncompressed_file_size = 0;
	uint32_t object_count = 0;
	uint32_t application_build = 0;
	SYSTEMTIME measurement_start_time = {};
	SYSTEMTIME last_object_time = {};
	uint64_t restore_points_offset = 0;
	uint32_t reserved_file_statistics[32] = {};
};

struct  LogContainer
{
	ObjectHeaderBase  header_base;
	uint16_t compression_method;
	uint16_t reservedLog_container1;
	uint32_t reservedLog_container2;
	uint32_t uncompressed_file_size;
	uint32_t reserved_log_container3;
	uint8_t compressed_file[4 * 1024 * 1024];
};

#pragma pack(pop)
#endif //INTERNAL_OBJECT_H
