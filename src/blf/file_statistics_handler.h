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
	file_statistics_.restore_points_offset = 0;
	memset(file_statistics_.reserved_file_statistics, 0, sizeof(file_statistics_.reserved_file_statistics));
}

inline SYSTEMTIME posix_us_to_systemtime(uint64_t posix_us)
{
	SYSTEMTIME st{};

	// 1) 拆分秒和毫秒
	time_t sec = (time_t)(posix_us / 1000000ULL);
	uint32_t ms = (uint32_t)((posix_us % 1000000ULL) / 1000ULL);

	// 2) 转 tm（线程安全：优先用 gmtime_r/localtime_r；Windows 可用 gmtime_s/localtime_s）
	std::tm tmv{};

#if defined(_WIN32)
	localtime_s(&tmv, &sec);
#else
	localtime_r(&sec, &tmv);
#endif

	// 3) 填 SYSTEMTIME
	st.year         = (uint16_t)(tmv.tm_year + 1900);
	st.month        = (uint16_t)(tmv.tm_mon + 1);
	st.dayOfWeek    = (uint16_t)(tmv.tm_wday);     // 0=Sunday
	st.day          = (uint16_t)(tmv.tm_mday);
	st.hour         = (uint16_t)(tmv.tm_hour);
	st.minute       = (uint16_t)(tmv.tm_min);
	st.second       = (uint16_t)(tmv.tm_sec);
	st.milliseconds = (uint16_t)ms;

	return st;
}

inline bool FileStatisticsHandler::write_file_header(FileWriter& writer)
{
	auto now = get_posix_time_us_uint64();
	printf("FileStatisticsHandler::write_file_header now time : %llu\n", now);
	file_statistics_.measurement_start_time = posix_us_to_systemtime(now);
	file_statistics_.last_object_time = posix_us_to_systemtime(now);
	writer.set_file_start_time(now);

	writer.write_struct(file_statistics_);
	return true;
}

inline bool FileStatisticsHandler::update_file_header(FileWriter& writer)
{
	file_statistics_.uncompressed_file_size = file_statistics_.statistics_size;
	file_statistics_.last_object_time = posix_us_to_systemtime(get_posix_time_us_uint64());

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
