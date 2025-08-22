#ifndef LIN_SHORT_OR_SLOW_RESPONSE2_H
#define LIN_SHORT_OR_SLOW_RESPONSE2_H

#include "../common.h"
#include "object_header.h"
#include "lin_databyte_timestamp_event.h"

BLFSTART
class TWBLF_API LinShortOrSlowResponse2 : public ObjectHeader
{
public:
    LinDatabyteTimestampEvent lin_timestamp_event;
    uint32_t number_of_resp_bytes {};
    std::array<uint8_t, 9> resp_bytes {};
    uint8_t slow_response {};
    uint8_t interrupted_by_break {};
    uint8_t reserved {};
    double exact_header_baudrate {};
    uint32_t early_stopbit_offset {};

    [[nodiscard]] uint32_t lin_short_or_slow_response2_object_size() const
    {
        return object_header_object_size() +
               lin_timestamp_event.lin_databyte_timestamp_event_object_size() +
               sizeof(number_of_resp_bytes) +
               sizeof(resp_bytes) +
               sizeof(slow_response) +
               sizeof(interrupted_by_break) +
               sizeof(reserved) +
               sizeof(exact_header_baudrate) +
               sizeof(early_stopbit_offset);
    }
};
BLFEND

#endif //LIN_SHORT_OR_SLOW_RESPONSE2_H
