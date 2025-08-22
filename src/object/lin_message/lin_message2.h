#ifndef LIN_MESSAGE2_H
#define LIN_MESSAGE2_H

#include "../common.h"
#include "object_header.h"
#include "lin_databyte_timestamp_event.h"
#include <array>

BLFSTART
class TWBLF_API LinMessage2 : public ObjectHeader
{
public:
    LinDatabyteTimestampEvent lin_timestamp_event;
    std::array<uint8_t, 8> data {};
    uint16_t crc {};
    uint8_t dir {};
    uint8_t simulated {};
    uint8_t is_etf {};
    uint8_t etf_assoc_index {};
    uint8_t etf_assoc_etf_id {};
    uint8_t fsm_id {};
    uint8_t fsm_state {};
    std::array<uint8_t, 3> reserved {};
    uint32_t resp_baudrate {};
    double exact_header_baudrate {};
    uint32_t early_stopbit_offset {};
    uint32_t early_stopbit_offset_response {};

    [[nodiscard]] uint32_t lin_message2_object_size() const
    {
        return object_header_object_size() +
               lin_timestamp_event.lin_databyte_timestamp_event_object_size() +
               sizeof(data) +
               sizeof(crc) +
               sizeof(dir) +
               sizeof(simulated) +
               sizeof(is_etf) +
               sizeof(etf_assoc_index) +
               sizeof(etf_assoc_etf_id) +
               sizeof(fsm_id) +
               sizeof(fsm_state) +
               sizeof(reserved) +
               sizeof(resp_baudrate) +
               sizeof(exact_header_baudrate) +
               sizeof(early_stopbit_offset) +
               sizeof(early_stopbit_offset_response);
    }
};
BLFEND

#endif //LIN_MESSAGE2_H
