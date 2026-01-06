#ifndef BLF_STRUCTURE_H
#define BLF_STRUCTURE_H

#include <cstdint>
#include <ios>
#include "blf_object_header.h"
#include "../include/can_object.h"

#define BUFFER_MAX_SIZE (4 * 1024 * 1024)

namespace BLF
{

#pragma pack(push, 1)

struct SYSTEMTIME {
    uint16_t year;
    uint16_t month;
    uint16_t dayOfWeek;
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
    uint32_t reserved_file_statistics[32];
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

struct BlfCanMessage {
    ObjectHeaderBase header_base;
    ObjectHeader header;
    CanFrame data;
};

#pragma pack(pop)

}

#endif //BLF_STRUCTURE_H
