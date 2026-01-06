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
	static SYSTEMTIME getCurrentSystemTime();

	FileStatistics file_statistics_{};
};

inline SYSTEMTIME FileStatisticsHandler::getCurrentSystemTime() {
	SYSTEMTIME st = {};
	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);
	struct tm* timeinfo = localtime(&time);
	st.year = timeinfo->tm_year + 1900;
	st.month = timeinfo->tm_mon + 1;
	st.dayOfWeek = timeinfo->tm_wday;
	st.day = timeinfo->tm_mday;
	st.hour = timeinfo->tm_hour;
	st.minute = timeinfo->tm_min;
	st.second = timeinfo->tm_sec;
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		now.time_since_epoch() % std::chrono::seconds(1));
	st.milliseconds = ms.count();
	return st;
}

inline FileStatisticsHandler::FileStatisticsHandler()
{
	file_statistics_.signature = BL_FILE_SIGNATURE;
	file_statistics_.statistics_size = sizeof(FileStatistics);
	file_statistics_.api_number = 4080200;
	file_statistics_.application_id = 5;//0;
	file_statistics_.compression_level = 0;
	file_statistics_.application_major = 0;
	file_statistics_.application_minor = 0;
	file_statistics_.file_size = 0;
	file_statistics_.uncompressed_file_size = 0;
	file_statistics_.object_count = 0;
	file_statistics_.application_build = 0;
	file_statistics_.measurement_start_time = getCurrentSystemTime();
	file_statistics_.last_object_time = getCurrentSystemTime();
	file_statistics_.restore_points_offset = 0;
	memset(file_statistics_.reserved_file_statistics, 0, sizeof(file_statistics_.reserved_file_statistics));
}

inline bool FileStatisticsHandler::write_file_header(FileWriter& writer)
{
	writer.write_struct(file_statistics_);
	return true;
}

inline bool FileStatisticsHandler::update_file_header(FileWriter& writer)
{
	file_statistics_.uncompressed_file_size = file_statistics_.statistics_size;
	file_statistics_.last_object_time = getCurrentSystemTime();

	writer.write_struct(file_statistics_);
	return true;
}

inline void FileStatisticsHandler::set_compres_level(int32_t compres_level)
{
	file_statistics_.compression_level = compres_level;
}

inline void FileStatisticsHandler::update_frame_count(int32_t frame_count)
{
	file_statistics_.object_count = frame_count;
}

inline void FileStatisticsHandler::update_file_size(uint64_t file_size)
{
	file_statistics_.file_size = file_size;
}

}

#endif //FILE_STATISTICS_H
