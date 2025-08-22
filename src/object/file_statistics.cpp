#include "file_statistics.h"
#include "abstract_file.h"

BLFSTART

void FileStatistics::read(AbstractFile& is)
{
    is.read(reinterpret_cast<char*>(&signature), sizeof(signature));
    is.read(reinterpret_cast<char*>(&statistics_size), sizeof(statistics_size));
    is.read(reinterpret_cast<char*>(&api_number), sizeof(api_number));
    is.read(reinterpret_cast<char*>(&application_id), sizeof(application_id));
    is.read(reinterpret_cast<char*>(&compression_level), sizeof(compression_level));
    is.read(reinterpret_cast<char*>(&application_major), sizeof(application_major));
    is.read(reinterpret_cast<char*>(&application_minor), sizeof(application_minor));
    is.read(reinterpret_cast<char*>(&file_size), sizeof(file_size));
    is.read(reinterpret_cast<char*>(&uncompressed_file_size), sizeof(uncompressed_file_size));
    is.read(reinterpret_cast<char*>(&object_count), sizeof(object_count));
    is.read(reinterpret_cast<char*>(&application_build), sizeof(application_build));
    is.read(reinterpret_cast<char*>(&measurement_start_time), sizeof(measurement_start_time));
    is.read(reinterpret_cast<char*>(&last_object_time), sizeof(last_object_time));
    is.read(reinterpret_cast<char*>(&restore_points_offset), sizeof(restore_points_offset));
    is.read(reinterpret_cast<char*>(&reserved_file_statistics), sizeof(reserved_file_statistics));
}

void FileStatistics::write(AbstractFile& os)
{
    os.write(reinterpret_cast<const char*>(&signature), sizeof(signature));
    os.write(reinterpret_cast<const char*>(&statistics_size), sizeof(statistics_size));
    os.write(reinterpret_cast<const char*>(&api_number), sizeof(api_number));
    os.write(reinterpret_cast<const char*>(&application_id), sizeof(application_id));
    os.write(reinterpret_cast<const char*>(&compression_level), sizeof(compression_level));
    os.write(reinterpret_cast<const char*>(&application_major), sizeof(application_major));
    os.write(reinterpret_cast<const char*>(&application_minor), sizeof(application_minor));
    os.write(reinterpret_cast<const char*>(&file_size), sizeof(file_size));
    os.write(reinterpret_cast<const char*>(&uncompressed_file_size), sizeof(uncompressed_file_size));
    os.write(reinterpret_cast<const char*>(&object_count), sizeof(object_count));
    os.write(reinterpret_cast<const char*>(&application_build), sizeof(application_build));
    os.write(reinterpret_cast<const char*>(&measurement_start_time), sizeof(measurement_start_time));
    os.write(reinterpret_cast<const char*>(&last_object_time), sizeof(last_object_time));
    os.write(reinterpret_cast<const char*>(&restore_points_offset), sizeof(restore_points_offset));
    os.write(reinterpret_cast<const char*>(&reserved_file_statistics), sizeof(reserved_file_statistics));
}

#define FILE_STATISTICS_SIZE \
(sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint8_t) + sizeof(uint8_t) + \
sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint64_t) + sizeof(uint64_t) + sizeof(uint32_t) + \
sizeof(uint32_t) + SYSTEMTIME_SIZE + SYSTEMTIME_SIZE + sizeof(uint64_t) + (sizeof(uint32_t) * 16) )

uint32_t FileStatistics::file_statistics_object_size() const
{
    return sizeof(signature) +
        sizeof(statistics_size) +
        sizeof(api_number) +
        sizeof(application_id) +
        sizeof(compression_level) +
        sizeof(application_major) +
        sizeof(application_minor) +
        sizeof(file_size) +
        sizeof(uncompressed_file_size) +
        sizeof(object_count) +
        sizeof(application_build) +
        sizeof(measurement_start_time) +
        sizeof(last_object_time) +
        sizeof(restore_points_offset) +
        sizeof(reserved_file_statistics);
}

BLFEND