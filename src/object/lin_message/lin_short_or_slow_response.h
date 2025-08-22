#ifndef LIN_SHORT_OR_SLOW_RESPONSE_H
#define LIN_SHORT_OR_SLOW_RESPONSE_H

#include "../common.h"
#include "object_header.h"
#include "lin_databyte_timestamp_event.h"

BLFSTART
class TWBLF_API LinShortOrSlowResponse : public ObjectHeader
{
public:
    LinDatabyteTimestampEvent mLinTimestampEvent;
    uint32_t numberOfRespBytes {};
    std::array<uint8_t, 9> respBytes {};
    uint8_t slowResponse {};
    uint8_t interruptedByBreak {};
    uint8_t reserved {};

    [[nodiscard]] uint32_t lin_short_or_slow_response_object_size() const
    {
        return object_header_object_size() +
               mLinTimestampEvent.lin_databyte_timestamp_event_object_size() +
               sizeof(numberOfRespBytes) +
               sizeof(respBytes) +
               sizeof(slowResponse) +
               sizeof(interruptedByBreak) +
               sizeof(reserved);
    }
};
BLFEND

#endif //LIN_SHORT_OR_SLOW_RESPONSE_H
