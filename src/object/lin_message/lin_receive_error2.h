#ifndef LIN_RECEIVE_ERROR2_H
#define LIN_RECEIVE_ERROR2_H

#include "../common.h"
#include "object_header.h"
#include "lin_databyte_timestamp_event.h"
#include <array>

BLFSTART
class TWBLF_API LinReceiveError2 : public ObjectHeader
{
public:
    LinDatabyteTimestampEvent lin_timestamp_event;
    std::array<uint8_t, 8> data {};
    uint8_t fsm_id {};
    uint8_t fsm_state {};
    uint8_t state_reason {};
    uint8_t offending_byte {};
    uint8_t short_error {};
    uint8_t timeout_during_dlc_detection {};
    uint8_t is_etf {};
    uint8_t has_databytes {};
    uint32_t resp_baudrate {};
    std::array<uint8_t, 4> reserved {};
    double exact_header_baudrate {};
    uint32_t early_stopbit_offset {};
    uint32_t early_stopbit_offset_response {};

    [[nodiscard]] uint32_t lin_receive_error2_object_size() const
    {
        return object_header_object_size() +
               lin_timestamp_event.lin_databyte_timestamp_event_object_size() +
               sizeof(data) +
               sizeof(fsm_id) +
               sizeof(fsm_state) +
               sizeof(state_reason) +
               sizeof(offending_byte) +
               sizeof(short_error) +
               sizeof(timeout_during_dlc_detection) +
               sizeof(is_etf) +
               sizeof(has_databytes) +
               sizeof(resp_baudrate) +
               sizeof(reserved) +
               sizeof(exact_header_baudrate) +
               sizeof(early_stopbit_offset) +
               sizeof(early_stopbit_offset_response);
    }
};
BLFEND

#endif //LIN_RECEIVE_ERROR2_H
