#ifndef BLF_LOGGER_H
#define BLF_LOGGER_H

#include "../include/logger.h"
#include "imessage_writer.h"
#include "../api/logger_registrar.h"
#include "file_statistics_writer.h"
#include "log_container_writer.h"

#include <fstream>
#include <map>

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

	void flush() override;

	bool write(const BusMessage& msg) override;

	void set_compres_level(int32_t compres_level) override;

private:
	std::ofstream file_;
	std::map<BusType, std::unique_ptr<IMessageWriter>> writer_;
	FileWriter file_writer_;
	FileStatisticsWriter file_statistics_writer_;
	LogContainerWriter log_container_writer_;

	int32_t frame_count_;	// 保存帧数量
};


static LoggerRegistrar<BlfLogger> registrar(FileFormat::BLF);


}

#endif //BLF_LOGGER_H
