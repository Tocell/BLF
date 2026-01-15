#ifndef BLF_LOGGER_H
#define BLF_LOGGER_H

#include "../include/logger.h"
#include "imessage_writer.h"
#include "../registry/logger_registrar.h"
#include "file_statistics_handler.h"
#include "log_container_handler.h"

#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <queue>
#include <vector>

#include "file_reader.h"

#define MAX_FRAME_CACHE_COUNT (300 * 1000)

namespace BLF
{

class BlfLogger : public Logger
{
public:
	BlfLogger();

	~BlfLogger() override = default;

	bool open(const std::string& filepath, OpenMode mode) override;
	void close() override;
	[[nodiscard]] bool is_open() const override;
	[[nodiscard]] uint64_t get_message_count() const override;
	[[nodiscard]] uint64_t get_file_size() const override;
	bool write(BusMessagePtr msg) override;
	void set_compres_level(int32_t compres_level) override;
	void flush_logcontainer(LogContainer& log_container);
	void set_timestamp_unit(int32_t unit) override;

	void read(BusMessagePtr& msg) override;

	void get_measure_time(uint64_t& start_time, uint64_t& stop_time) override;

	uint32_t align_pad_like_writer(uint32_t object_size);

	void writer_thread_handler();

	void read_logcontainer_thread_handler();

	void read_busmsg_thread_handler();

private:
	struct LogContainerBlock {
		uint16_t compression_method{};
		uint32_t uncompressed_size{};
		std::vector<uint8_t> uncompressed; // size == uncompressed_size
	};

private:
	std::map<BusType, std::unique_ptr<IMessageWriter>> writer_;
	OpenMode mode_{};
	FileWriter file_writer_;

	FileReader file_reader_;
	bool eof_{false};

	FileStatisticsHandler file_statistics_writer_;
	LogContainerHandler log_container_;

	std::atomic<int32_t> frame_count_;	// 保存帧数量
	int32_t compression_method_;
	int32_t compression_level_;

	std::mutex msg_mtx_;
	std::condition_variable msg_cv_;
	std::thread writer_thread_;
	std::atomic<bool> is_running_;

	std::thread read_logcontainer_thread_;
	std::thread read_busmsg_thread_;

	std::queue<BusMessagePtr> msg_queue_;

	std::mutex log_mtx_;
	std::condition_variable log_cv_;
	std::queue<LogContainerBlock> log_queue_;
};


static LoggerRegistrar<BlfLogger> registrar(FileFormat::BLF);


}

#endif //BLF_LOGGER_H
