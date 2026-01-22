#ifndef BLF_LOGGER_CAN_MESSAGE_ASC_READER_H
#define BLF_LOGGER_CAN_MESSAGE_ASC_READER_H
#include "iasc_message_reader.h"
#include "asc_reader_registrar.h"
#include "frame_type_define.h"
#include "file_reader.h"

namespace GWLogger::Asc
{

class CanMessageAscReader : public IAscMessageReader
{
public:
    ~CanMessageAscReader() override = default;

    [[nodiscard]] BusMessagePtr read_line(const std::string& line,
                            uint64_t file_start_posix_us) const override;

	[[nodiscard]] bool match(const std::string& line) const override;
};

}

#endif //BLF_LOGGER_CAN_MESSAGE_ASC_READER_H