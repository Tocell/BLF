#ifndef BLF_LOGGER_H
#define BLF_LOGGER_H

#include "../include/logger.h"
#include "imessage_writer.h"

#include <fstream>
#include <map>

namespace BLF
{

class BlfLogger : public Logger
{
public:
	~BlfLogger() override = default;

	bool open(const std::string& filepath, bool append) override;

	void close() override;

	bool write(const BusMessage& msg) override;

	[[nodiscard]] bool is_open() const override;

	[[nodiscard]] uint64_t get_message_count() const override;

	[[nodiscard]] uint64_t get_file_size() const override;

	void flush() override;

private:
	void register_writer();

	std::ofstream file_;
	std::map<BusType, std::unique_ptr<IMessageWriter>> writer_;
};

}

#endif //BLF_LOGGER_H
