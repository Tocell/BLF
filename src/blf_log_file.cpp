#include "blf_log_file.h"

#include "log.h"
#include "internal_objects.h"

#include <chrono>

BlfLogFile::~BlfLogFile()
{
	if (file_.is_open())
	{
		file_.close();
	}
};

static SYSTEMTIME getCurrentSystemTime() {
	SYSTEMTIME st = {};
	auto now = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(now);
	std::tm timeinfo = {};
	#if defined(_WIN32) || defined(_WIN64)
	localtime_s(&timeinfo, &time);
	#else
	localtime_r(&time, &timeinfo);
	#endif

	st.year = timeinfo.tm_year + 1900;
	st.month = timeinfo.tm_mon + 1;
	st.dayOf_week = timeinfo.tm_wday;
	st.day = timeinfo.tm_mday;
	st.hour = timeinfo.tm_hour;
	st.minute = timeinfo.tm_min;
	st.second = timeinfo.tm_sec;

	auto duration = now.time_since_epoch();
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;
	st.milliseconds = static_cast<uint16_t>(millis.count());
	return st;
}

void BlfLogFile::openFile(const std::string& filepath)
{
	if (file_.is_open())
	{
		file_.close();
	}
	file_.open(filepath, std::ios::out | std::ios::app);
	if (!file_.is_open())
	{
		LOG_ERROR("Open file {} failed.", filepath.c_str());
	}

	FileStatistics file_stats;
	file_stats.statistics_size = sizeof(FileStatistics);
	file_stats.measurement_start_time = getCurrentSystemTime();
	file_stats.last_object_time = file_stats.measurement_start_time;
	file_stats.object_count = 1;

}

void BlfLogFile::closeFile()
{
	if (file_.is_open())
	{
		file_.close();
	}
	// todo 更新 文件开头的 FileStatistics 中的 last_object_time 和 object_count 以及 file_size
}