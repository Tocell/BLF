#ifndef FILESTATISTICS_H
#define FILESTATISTICS_H
#include <cstdint>
#include <chrono>
#include <ctime>
#include "common.h"
#include "abstract_file.h"

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

const uint32_t FileSignature = 0x47474F4C; /* LOGG */

enum ApplicationId : uint8_t {
    Unknown = 0,
    Canalyzer = 1,
    Canoe = 2,
    Canstress = 3,
    Canlog = 4,
    Canape = 5,
    Cancasexllog = 6,
    Vlconfig = 7,
    Porschelogger = 200,
    Caeteclogger = 201,
    Vectornetworksimulator = 202,
    Ipetroniklogger=203,
    RtPk=204,
    Piketec=205,
    Sparks=206
};

BLFSTART
class TWBLF_API FileStatistics final {
public:
    FileStatistics() = default;
    virtual ~FileStatistics() noexcept = default;
    FileStatistics(const FileStatistics&) = default;
    FileStatistics& operator=(const FileStatistics&) = default;

    virtual void read(AbstractFile& is);
    // 写入物理文件中文件头部数据
    virtual void write(AbstractFile& os);

    [[nodiscard]] uint32_t file_statistics_object_size() const;

    uint32_t    signature = 0x47474F4C;
    uint32_t    statistics_size = 0;
    uint32_t    api_number = 4080200;  // 4.8.2.0
    uint8_t     application_id = 0;
    uint8_t     compression_level = 0;  // No compression
    uint8_t     application_major = 0;
    uint8_t     application_minor = 0;
    uint64_t    file_size = 0;
    uint64_t    uncompressed_file_size = 0;
    uint32_t    object_count = 0;
    uint32_t    application_build = 0;
    SYSTEMTIME  measurement_start_time = {};
    SYSTEMTIME  last_object_time = {};
    uint64_t    restore_points_offset = 0;
    uint32_t    reserved_file_statistics[16] = {};
};

BLFEND

#endif //FILESTATISTICS_H
