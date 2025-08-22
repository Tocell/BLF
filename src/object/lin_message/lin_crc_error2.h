#ifndef LIN_CRC_ERROR2_H
#define LIN_CRC_ERROR2_H

#include "../common.h"
#include "object_header.h"
#include "lin_databyte_timestamp_event.h"
#include <array>

BLFSTART
class TWBLF_API LinCrcError2 : public ObjectHeader
{
public:
    LinDatabyteTimestampEvent lin_timestamp_event;
    std::array<uint8_t, 8> data {};
    uint16_t crc {};
    uint8_t dir {};
    uint8_t fsm_id {};
    uint8_t fsm_state {};
    uint8_t simulated {};
    std::array<uint8_t, 2> reserved1 {};
    uint32_t resp_baudrate {};
    std::array<uint8_t, 4> reserved2 {};
    double exact_header_baudrate {};
    uint32_t early_stopbit_offset {};
    uint32_t early_stopbit_offset_response {};

    [[nodiscard]] uint32_t lin_crc_error2_object_size() const
    {
        return object_header_object_size() +
               lin_timestamp_event.lin_databyte_timestamp_event_object_size() +
               sizeof(data) +
               sizeof(crc) +
               sizeof(dir) +
               sizeof(fsm_id) +
               sizeof(fsm_state) +
               sizeof(simulated) +
               sizeof(reserved1) +
               sizeof(resp_baudrate) +
               sizeof(reserved2) +
               sizeof(exact_header_baudrate) +
               sizeof(early_stopbit_offset) +
               sizeof(early_stopbit_offset_response);
    }
};
BLFEND

#endif //LIN_CRC_ERROR2_H
