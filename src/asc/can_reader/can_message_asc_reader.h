#ifndef BLF_LOGGER_CAN_MESSAGE_ASC_READER_H
#define BLF_LOGGER_CAN_MESSAGE_ASC_READER_H

#include "../../api/iasc_message_reader.h"
#include "../../registry/asc_reader_registrar.h"
// #include "../blf_object_header.h"
#include "../../io/file_reader.h"

namespace BLF
{

class CanMessageAscReader : public IAscMessageReader
{
public:
    // CanMessageAscReader();
    ~CanMessageAscReader() override = default;

    [[nodiscard]] bool match(const std::string& line) const override;

    [[nodiscard]] BusMessagePtr read_line(const std::string& line,
                            uint64_t file_start_posix_us) const override;

};

}

#endif //BLF_LOGGER_CAN_MESSAGE_ASC_READER_H