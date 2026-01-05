#ifndef ASC_LOGGER_H
#define ASC_LOGGER_H

#include "../include/logger.h"
#include "imessage_writer.h"

#include <fstream>
#include <map>

namespace BLF
{
class AscLogger : public Logger
{
public:
	AscLogger();

	~AscLogger() override = default;

	bool open(const std::string& filepath, int32_t mode, bool append) override;

	void close() override;

	[[nodiscard]] bool is_open() const override;

	[[nodiscard]] uint64_t get_message_count() const override;

	[[nodiscard]] uint64_t get_file_size() const override;

	// std::unique_ptr<BusMessage> read() override;
	bool write(const BusMessage& msg) override;

	void flush() override;

	void set_compres_level(int32_t compres_level) override;

private:
	void register_writer();

	std::ofstream file_;
	std::map<BusType, std::unique_ptr<IMessageWriter>> writer_;
	FileWriter file_writer_;
};

}

#endif //ASC_LOGGER_H
