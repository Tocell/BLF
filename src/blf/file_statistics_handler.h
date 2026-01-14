#ifndef FILE_STATISTICS_H
#define FILE_STATISTICS_H
#include "../api/imessage_writer.h"
#include "../include/types.h"
#include "../io/file_writer.h"
#include "blf_structure.h"

#include <chrono>
#include <iostream>

namespace BLF
{

inline uint64_t get_posix_time_us_uint64()
{
	auto now = std::chrono::system_clock::now();
	return std::chrono::time_point_cast<std::chrono::microseconds>(now).time_since_epoch().count();
}

class FileStatisticsHandler
{
public:
	FileStatisticsHandler();

	~FileStatisticsHandler() = default;

	void set_compres_level(int32_t compres_level);

	bool write_file_header(FileWriter& writer);

	bool update_file_header(FileWriter& writer);

	void update_frame_count(int32_t frame_count);

	void update_file_size(uint64_t file_size);

private:
	FileStatistics file_statistics_{};
};

}

#endif //FILE_STATISTICS_H
