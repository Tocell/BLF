#ifndef BLF_LOGGER_IASC_MESSAGE_READER_H
#define BLF_LOGGER_IASC_MESSAGE_READER_H

#include <string>

#include "message_factory.h"

namespace GWLogger::Asc{

class IAscMessageReader
{
public:
    virtual ~IAscMessageReader() = default;

    // [[nodiscard]] virtual uint32_t key() const = 0;

    [[nodiscard]] virtual BusMessagePtr read_line(const std::string& line, uint64_t file_start_time) const = 0;

    [[nodiscard]] virtual bool match(const std::string& line) const = 0;
};

}

#endif //BLF_LOGGER_IASC_MESSAGE_READER_H