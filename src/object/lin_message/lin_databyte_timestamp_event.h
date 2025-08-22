#ifndef LIN_DATABYTE_TIMESTAMP_EVENT_H
#define LIN_DATABYTE_TIMESTAMP_EVENT_H

#include "../common.h"
#include "object_header.h"
#include "lin_message_descriptor.h"
#include <array>

BLFSTART
class TWBLF_API LinDatabyteTimestampEvent : public LinMessageDescriptor
{
public:
    std::array<uint64_t, 9> databyte_timestamps {};

    [[nodiscard]] uint32_t lin_databyte_timestamp_event_object_size() const
    {
        return lin_message_descriptor_object_size() + sizeof(databyte_timestamps);
    }
};
BLFEND

#endif //LIN_DATABYTE_TIMESTAMP_EVENT_H
