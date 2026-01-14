#include "file_statistics_handler.h"

namespace BLF
{

FileStatisticsHandler::FileStatisticsHandler()
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

SYSTEMTIME posix_us_to_systemtime(uint64_t posix_us)
{
	SYSTEMTIME st{};
	auto sec = (time_t)(posix_us / 1000000ULL);
	auto ms = (uint32_t)((posix_us % 1000000ULL) / 1000ULL);

	std::tm tmv{};

#if defined(_WIN32)
	localtime_s(&tmv, &sec);
#else
	localtime_r(&sec, &tmv);
#endif

	st.year         = (uint16_t)(tmv.tm_year + 1900);
	st.month        = (uint16_t)(tmv.tm_mon + 1);
	st.day_of_week  = (uint16_t)(tmv.tm_wday);     // 0=Sunday
	st.day          = (uint16_t)(tmv.tm_mday);
	st.hour         = (uint16_t)(tmv.tm_hour);
	st.minute       = (uint16_t)(tmv.tm_min);
	st.second       = (uint16_t)(tmv.tm_sec);
	st.milliseconds = (uint16_t)ms;

	return st;
}

bool FileStatisticsHandler::write_file_header(FileWriter& writer)
{
	auto now = get_posix_time_us_uint64();
	printf("FileStatisticsHandler::write_file_header now time : %llu\n", now);
	file_statistics_.measurement_start_time = posix_us_to_systemtime(now);
	file_statistics_.last_object_time = posix_us_to_systemtime(now);
	writer.set_file_start_time(now);

	writer.write_struct(file_statistics_);
	return true;
}

bool FileStatisticsHandler::update_file_header(FileWriter& writer)
{
	file_statistics_.uncompressed_file_size = file_statistics_.statistics_size;
	file_statistics_.last_object_time = posix_us_to_systemtime(get_posix_time_us_uint64());

	writer.write_struct(file_statistics_);
	return true;
}

void FileStatisticsHandler::set_compres_level(int32_t compres_level)
{
	file_statistics_.compression_level = compres_level;
}

void FileStatisticsHandler::update_frame_count(int32_t frame_count)
{
	file_statistics_.object_count = frame_count;
}

void FileStatisticsHandler::update_file_size(uint64_t file_size)
{
	file_statistics_.file_size = file_size;
}

}