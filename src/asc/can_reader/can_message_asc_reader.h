#ifndef BLF_LOGGER_CAN_MESSAGE_ASC_READER_H
#define BLF_LOGGER_CAN_MESSAGE_ASC_READER_H

#include "../../api/iasc_message_reader.h"
#include "../../registry/asc_reader_registrar.h"
#include "../include/frame_type_define.h"
#include "../../io/file_reader.h"

namespace BLF
{

class CanMessageAscReader : public IAscMessageReader
{
public:
    // CanMessageAscReader();
    ~CanMessageAscReader() override = default;

	static constexpr uint32_t kKey = BL_OBJ_TYPE_CAN_MESSAGE;

	uint32_t key() const override;

    [[nodiscard]] BusMessagePtr read_line(const std::string& line,
                            uint64_t file_start_posix_us) const override;

};

}

#endif //BLF_LOGGER_CAN_MESSAGE_ASC_READER_H