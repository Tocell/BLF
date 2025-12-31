// #ifndef ASC_LOGGER_H
// #define ASC_LOGGER_H
// #include "../include/logger.h"
//
// namespace BLF
// {
// class AscLogger : public Logger
// {
// public:
// 	~AscLogger() override = default;
//
// 	bool open(const std::string& filepath, bool append) override;
//
// 	void close() override;
//
// 	[[nodiscard]] bool is_open() const override;
//
// 	[[nodiscard]] uint64_t get_message_count() const override;
//
// 	[[nodiscard]] uint64_t get_file_size() const override;
//
// 	// std::unique_ptr<BusMessage> read() override;
//
// 	void flush() override;
// };
//
// }
//
// #endif //ASC_LOGGER_H
