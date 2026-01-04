#ifndef LOGGER_H
#define LOGGER_H
#include <memory>
#include <string>

#include "types.h"

namespace BLF
{

class BLF_API Logger {
public:
    static std::unique_ptr<Logger> create(FileFormat format);

    virtual ~Logger() = default;

    virtual bool open(const std::string& filepath, bool append) = 0;

    virtual void close() = 0;

    [[nodiscard]] virtual bool is_open() const = 0;

    [[nodiscard]] virtual uint64_t get_message_count() const = 0;

    [[nodiscard]] virtual uint64_t get_file_size() const = 0;

    virtual void flush() = 0;

    virtual bool write(const BusMessage& msg) = 0;
};

}

#endif //LOGGER_H
