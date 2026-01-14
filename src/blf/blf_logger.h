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

#define MAX_FRAME_CACHE_COUNT (300 * 1000)

namespace BLF
{

class BlfLogger : public Logger
{
public:
	BlfLogger();

	~BlfLogger() override = default;

	bool open(const std::string& filepath, int32_t mode, bool append) override;
	void close() override;
	[[nodiscard]] bool is_open() const override;
	[[nodiscard]] uint64_t get_message_count() const override;
	[[nodiscard]] uint64_t get_file_size() const override;
	bool write(BusMessagePtr msg) override;
	void set_compres_level(int32_t compres_level) override;
	void flush_logcontainer(LogContainer& log_container);
	void set_timestamp_unit(int32_t unit) override;

private:
	std::map<BusType, std::unique_ptr<IMessageWriter>> writer_;
	FileWriter file_writer_;
	FileStatisticsHandler file_statistics_writer_;
	LogContainerHandler log_container_writer_;

	std::atomic<int32_t> frame_count_;	// 保存帧数量
	int32_t compression_method_;
	int32_t compression_level_;

	std::mutex mutex_;
	std::condition_variable cv_;
	std::thread writer_thread_;
	std::atomic<bool> is_running_;

	std::queue<BusMessagePtr> msg_queue_;
};


static LoggerRegistrar<BlfLogger> registrar(FileFormat::BLF);


}

#endif //BLF_LOGGER_H
