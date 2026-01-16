#include "asc_logger.h"
#include "can/can_message_asc_writer.h"
#include "../registry/writer_registry.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace BLF
{

inline uint64_t get_posix_time_us_uint64()
{
	auto now = std::chrono::system_clock::now();
	return std::chrono::time_point_cast<std::chrono::microseconds>(now).time_since_epoch().count();
}

AscLogger::AscLogger():
	frame_count_(0)
{
	writer_ = WriterRegistry::get_instance().create_writers(FileFormat::ASC);
}

bool AscLogger::open(const std::string& filepath, OpenMode mode)
{
	mode_ = mode;
	if (OpenMode::Write == mode)
	{
		file_writer_.open(filepath);
		writer_header();

		is_running_.store(true);
		writer_thread_ = std::thread([this](){ return writer_thread_handler(); });
	}
	else
	{
		file_reader_.open(filepath);
	}
	return true;
}

void AscLogger::close()
{
	if (OpenMode::Write == mode_)
	{
		is_running_.store(false);
		msg_cv_.notify_all();
		if (writer_thread_.joinable()) writer_thread_.join();

		if (file_writer_.get_pos() > 0)
		{
			file_writer_.write(file_writer_.get_buffer(), file_writer_.get_pos());
			file_writer_.set_pos(0);
			file_writer_.flush();
		}

		file_writer_.close();
	}
	else
	{
		file_reader_.close();
	}
}

bool AscLogger::is_open() const
{
	if (OpenMode::Write == mode_)
	{
		return file_writer_.is_open();
	}
	else
	{
		return file_reader_.is_open();
	}
}

uint64_t AscLogger::get_message_count() const
{
	return 0;
}

uint64_t AscLogger::get_file_size() const
{
	return 0;
}

bool AscLogger::write(BusMessagePtr msg)
{
	if (!msg || !file_writer_.is_open()) return false;

	{
		std::unique_lock lock(msg_mtx_);
		if (msg_queue_.size() >= MAX_FRAME_CACHE_COUNT)
		{
			auto next = std::chrono::steady_clock::now();
			constexpr auto period = std::chrono::microseconds(10);
			next += period;
			std::this_thread::sleep_until(next);
		}
	}

	std::unique_lock lock(msg_mtx_);
	msg_queue_.push(std::move(msg));
	msg_cv_.notify_one();

	return true;
}



void AscLogger::set_timestamp_unit(int32_t unit)
{

}

void AscLogger::get_measure_time(uint64_t& start_time, uint64_t& stop_time)
{

}

bool AscLogger::read(BusMessagePtr& msg)
{
	return true;
}

void AscLogger::writer_header()
{
	if (!file_writer_.is_open())
	{
		return;
	}

	// 以该值作为“文件记录起始时间”（POSIX us）
	start_measure_time_ = get_posix_time_us_uint64();
	file_writer_.set_file_start_time(start_measure_time_);

	// 用 start_measure_time_ 生成 header 的 date 行（而不是 system_clock::now()）
	const uint64_t start_us = start_measure_time_;
	const std::time_t sec = static_cast<std::time_t>(start_us / 1000000ULL);
	const uint32_t msec = static_cast<uint32_t>((start_us % 1000000ULL) / 1000ULL);

	std::tm tm{};
	#if defined(_WIN32)
	localtime_s(&tm, &sec);
	#else
	localtime_r(&sec, &tm);
	#endif

	std::ostringstream oss;

	// line 1: date ... (Fulltime 带毫秒)
	oss << "date " << std::put_time(&tm, "%a %b ")
		<< tm.tm_mday << ' '
		<< std::put_time(&tm, "%I:%M:%S")
		<< '.' << std::setw(3) << std::setfill('0') << msec
		<< ' ' << (tm.tm_hour < 12 ? "am" : "pm")
		<< ' ' << (tm.tm_year + 1900)
		<< "\r\n";

	// line 2: base ... timestamps ...
	const char* base = "hex";
	const char* timestamps_mode = "absolute";
	oss << "base " << base << " timestamps " << timestamps_mode << "\r\n";

	// line 3: internal events ...
	const bool internal_events_logged = true;
	oss << (internal_events_logged ? "internal events logged"
								   : "no internal events logged")
		<< "\r\n";

	// optional version line
	oss << "// version 7.0.0\r\n";

	const std::string header = oss.str();
	file_writer_.write(reinterpret_cast<const uint8_t*>(header.data()), header.size());
	file_writer_.flush();
}


void AscLogger::writer_thread_handler()
{
	constexpr auto kWakeInterval = std::chrono::microseconds(10);

	while (is_running_.load())
	{
		BusMessagePtr msg;
		std::queue<BusMessagePtr> msg_queue;
		{
			std::unique_lock lock(msg_mtx_);
			msg_cv_.wait_for(lock, kWakeInterval, [this]()
			{
				return !msg_queue_.empty() || !is_running_.load();
			});
			if (!is_running_.load() && msg_queue_.empty())
				break;
			if (msg_queue_.empty()) continue;

			msg_queue = std::move(msg_queue_);
		}

		while (!msg_queue.empty())
		{
			msg = std::move(msg_queue.front());
			const auto bus_type = msg->get_bus_type();
			auto it = writer_.find(bus_type);
			if (it == writer_.end()) continue;

			if ((file_writer_.get_pos() + 400) >= BUFFER_MAX_SIZE)
			{
				file_writer_.write(file_writer_.get_buffer(), file_writer_.get_pos());
				file_writer_.set_pos(0);
			}

			auto result = it->second->write(*msg, file_writer_);

			frame_count_.fetch_add(1);

			msg_queue.pop();
		}
	}
}


}
