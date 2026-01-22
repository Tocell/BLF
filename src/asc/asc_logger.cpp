#include "asc_logger.h"
#include "writer_registry.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "asc_reader_registrar.h"
#include "asc_reader_helper.h"

namespace GWLogger::Asc
{

inline uint64_t get_posix_time_us_uint64()
{
	auto now = std::chrono::system_clock::now();
	return std::chrono::time_point_cast<std::chrono::microseconds>(now).time_since_epoch().count();
}

AscLogger::AscLogger():
	frame_count_(0)
{
	writer_.reserve(200);
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
		read_header();

		is_running_.store(true);
		read_thread_ =
			std::thread(&AscLogger::reader_thread_handler, this);
	}
	return true;
}

void AscLogger::close()
{
	is_running_.store(false);
	msg_cv_.notify_all();
	if (writer_thread_.joinable())
		writer_thread_.join();
	if (read_thread_.joinable())
		read_thread_.join();

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

	return file_reader_.is_open();
}

uint64_t AscLogger::get_message_count() const
{
	std::unique_lock lk(msg_mtx_);
	return msg_queue_.size();
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


void AscLogger::set_timestamp_unit(TimeStampUnit unit)
{
	for (auto& it : writer_)
	{
		it.second->set_timestamp_unit(unit);
	}
}

void AscLogger::get_measure_time(uint64_t& start_time, uint64_t& stop_time)
{
	start_time = start_measure_time_;
	stop_time = stop_measure_time_;
}

bool AscLogger::read(BusMessagePtr& msg)
{
	std::unique_lock lock(msg_mtx_);
	msg_cv_.wait(lock, [&]{
		return !msg_queue_.empty() || file_eof_;
	});
	if (!msg_queue_.empty())
	{
		msg = std::move(msg_queue_.front());
		msg_queue_.pop();
		return true;
	}
	msg.reset();
	return false;
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

static inline void trim_cr(std::string& s)
{
    if (!s.empty() && s.back() == '\r') s.pop_back();
}

static inline int month_to_index(const std::string& m)
{
    // English + German month abbreviations from the spec examples
    static const std::unordered_map<std::string, int> kMap = {
        {"Jan",0},{"Feb",1},{"Mar",2},{"Apr",3},{"May",4},{"Jun",5},{"Jul",6},{"Aug",7},{"Sep",8},{"Oct",9},{"Nov",10},{"Dec",11},
        {"Mär",2},{"Mai",4},{"Okt",9},{"Dez",11}
    };
    auto it = kMap.find(m);
    return (it == kMap.end()) ? -1 : it->second;
}

static inline bool parse_time_hms_ms(const std::string& s, int& hh, int& mm, int& ss, int& msec)
{
    // supports: "09:21:13.159" or "09:21:13"
    hh = mm = ss = 0; msec = 0;
    const auto dot = s.find('.');
    const std::string hms = (dot == std::string::npos) ? s : s.substr(0, dot);
    const std::string ms  = (dot == std::string::npos) ? "" : s.substr(dot + 1);

    if (std::sscanf(hms.c_str(), "%d:%d:%d", &hh, &mm, &ss) != 3) return false;

    if (!ms.empty())
    {
        // 只取前三位毫秒（"159" -> 159；"1" -> 100；"12" -> 120 这种也兼容）
        int v = std::atoi(ms.c_str());
        if (ms.size() == 1) v *= 100;
        else if (ms.size() == 2) v *= 10;
        msec = v % 1000;
    }
    return true;
}

// 解析形如：date Wed Apr 16 09:21:13.159 am 2014
// 或德文：date Die Dez 21 11:29:01 2004
static inline bool parse_asc_date_line_to_posix_us(const std::string& line, uint64_t& out_posix_us)
{
    auto parts = split_ws(line);
    if (parts.size() < 5) return false;
    if (parts[0] != "date") return false;

    // parts: ["date", WeekDay, Month, Date, Fulltime, (am/pm)?, Year]
    const std::string& month_str = parts[2];
    const int mon = month_to_index(month_str);
    if (mon < 0) return false;

    const int day = std::atoi(parts[3].c_str());

    int hh=0, mm=0, ss=0, msec=0;
    if (!parse_time_hms_ms(parts[4], hh, mm, ss, msec)) return false;

    std::string ampm;
    int year_idx = 5;
    if (parts.size() >= 7 && (parts[5] == "am" || parts[5] == "pm"))
    {
        ampm = parts[5];
        year_idx = 6;

        // 转 24 小时
        if (ampm == "pm" && hh < 12) hh += 12;
        if (ampm == "am" && hh == 12) hh = 0;
    }

    if (year_idx >= (int)parts.size()) return false;
    const int year = std::atoi(parts[year_idx].c_str());

    std::tm tm{};
    tm.tm_year = year - 1900;
    tm.tm_mon  = mon;
    tm.tm_mday = day;
    tm.tm_hour = hh;
    tm.tm_min  = mm;
    tm.tm_sec  = ss;
    tm.tm_isdst = -1; // 让 mktime 自己判定夏令时

    const std::time_t t = std::mktime(&tm); // 本地时区
    if (t == (std::time_t)-1) return false;

    out_posix_us = static_cast<uint64_t>(t) * 1000000ULL + static_cast<uint64_t>(msec) * 1000ULL;
    return true;
}

static inline bool is_asc_comment_line(const std::string& s)
{
	size_t i = 0;
	while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
	return (i + 1 < s.size() && s[i] == '/' && s[i + 1] == '/');
}

	// 可选：解析 version 注释
static inline bool parse_asc_version_comment(const std::string& s, int& major, int& minor, int& patch)
{
	// 形如: "// version 7.0.0"
	// 简单做法：找 "version" 后的三段数字
	const auto pos = s.find("version");
	if (pos == std::string::npos) return false;

	// 从 "version" 后面开始找数字
	const char* p = s.c_str() + pos;
	while (*p && !std::isdigit(static_cast<unsigned char>(*p))) ++p;
	if (!*p) return false;

	int a=0,b=0,c=0;
	if (std::sscanf(p, "%d.%d.%d", &a, &b, &c) == 3)
	{
		major = a; minor = b; patch = c;
		return true;
	}
	return false;
}

void AscLogger::read_header()
{
	if (!file_reader_.is_open())
		return;

	std::string line1, line2, line3;

	if (!file_reader_.read_line(line1)) return;
	if (!file_reader_.read_line(line2)) return;
	if (!file_reader_.read_line(line3)) return;

	trim_cr(line1);
	trim_cr(line2);
	trim_cr(line3);

	// 解析 date 行 -> POSIX us
	uint64_t start_us = 0;
	if (!parse_asc_date_line_to_posix_us(line1, start_us))
		// 如果格式不符合预期，你可以选择抛异常/记录日志/或置 0
		start_measure_time_ = 0;
	else
		start_measure_time_ = start_us;

	while (true)
	{
		const uint64_t pos_before = file_reader_.tell();

		std::string line;
		if (!file_reader_.read_line(line))
		{
			break;
		}
		trim_cr(line);

		if (!is_asc_comment_line(line))
		{
			file_reader_.seek(pos_before);
			break;
		}
	}
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

			auto* w = create_writer(bus_type);
			if (!w)
			{
				msg_queue.pop();
				continue;
			}

			if ((file_writer_.get_pos() + 400) >= BUFFER_MAX_SIZE)
			{
				file_writer_.write(file_writer_.get_buffer(), file_writer_.get_pos());
				file_writer_.set_pos(0);
			}
			auto result = w->write(*msg, file_writer_);
			(void)result;

			frame_count_.fetch_add(1);

			msg_queue.pop();
		}
	}
}

void AscLogger::reader_thread_handler()
{
	std::string line;
	constexpr auto kWakeInterval = std::chrono::microseconds(100);

	std::unordered_map<AscLineKey, std::vector<std::unique_ptr<IAscMessageReader>>, AscLineKeyHash> reader_cache;

	while (is_running_.load())
	{
		{
			std::unique_lock lock(msg_mtx_);
			msg_cv_.wait_for(lock, kWakeInterval, [this]()
			{
				return msg_queue_.size() < MAX_FRAME_CACHE_COUNT || !is_running_.load();
			});
			if (!is_running_.load() || file_eof_.load()) break;
			if (msg_queue_.size() >= MAX_FRAME_CACHE_COUNT) continue;
		}
		if (!file_reader_.read_line(line))
		{
			if (file_reader_.eof() || file_reader_.has_error())
			{
				file_eof_.store(true);
				msg_cv_.notify_all();
				break;
			}
			continue;
		}

		if (line.empty() || is_asc_comment_line(line)) continue;

		auto* reader = AscReaderRegistry::instance().find_reader(line, reader_cache);
		if (!reader) continue;

		auto msg = reader->read_line(line, start_measure_time_);
		if (!msg) continue;
		{
			std::lock_guard qlk(msg_mtx_);
			msg_queue_.push(std::move(msg));
		}
		msg_cv_.notify_one();
	}
}

IMessageWriter* AscLogger::create_writer(BusType bus_type)
{
	if (auto it = writer_.find(bus_type); it != writer_.end())
		return it->second.get();

	const auto* fac = WriterRegistry::get_instance().find_writer(FileFormat::ASC, bus_type);
	if (!fac) return nullptr;

	auto [it, inserted] = writer_.emplace(bus_type, std::move((*fac)()));
	return it->second.get();
}

}
