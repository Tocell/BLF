#ifndef ASC_LOGGER_H
#define ASC_LOGGER_H

#include "logger.h"
#include "imessage_writer.h"
#include "logger_registrar.h"
#include "file_reader.h"

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

namespace GWLogger::Asc
{
class AscLogger : public Logger
{
public:
	AscLogger();
	~AscLogger() override = default;

	bool open(const std::string& filepath, OpenMode mode) override;
	void close() override;

	[[nodiscard]] bool is_open() const override;
	[[nodiscard]] uint64_t get_message_count() const override;
	[[nodiscard]] uint64_t get_file_size() const override;

	void set_compres_level(int32_t compres_level) override { };
	void set_timestamp_unit(int32_t unit) override;

	bool write(BusMessagePtr msg) override;
	bool read(BusMessagePtr& msg) override;

	void get_measure_time(uint64_t& start_time, uint64_t& stop_time) override;

	void writer_header();

	void read_header();

	void writer_thread_handler();

	void reader_thread_handler();

private:
	std::unordered_map<BusType, std::unique_ptr<IMessageWriter>> writer_;
	IMessageWriter* create_writer(BusType bus_type);

	OpenMode mode_{};
	FileWriter file_writer_;

	FileReader file_reader_;

	uint64_t start_measure_time_{};
	uint64_t stop_measure_time_{};

	std::mutex msg_mtx_;
	std::condition_variable msg_cv_;
	std::queue<BusMessagePtr> msg_queue_;

	std::thread writer_thread_;
	std::atomic<bool> is_running_;

	std::atomic<int32_t> frame_count_;

	std::thread read_thread_;

	std::atomic<bool> file_eof_{false};
};

static LoggerRegistrar<AscLogger> registrar(FileFormat::ASC);

}

#endif //ASC_LOGGER_H
