#ifndef LIN_BAUDRATE_EVENT_H
#define LIN_BAUDRATE_EVENT_H

#include "../common.h"
#include "object_header.h"
#include <array>

BLFSTART
class TWBLF_API LinBaudrateEvent : public ObjectHeader
{
public:
    uint16_t channel {};
    uint16_t dummy {};
    int32_t baudrate {};

    [[nodiscard]] uint32_t lin_baudrate_event_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(dummy) +
               sizeof(baudrate);
    }
};
BLFEND

#endif //LIN_BAUDRATE_EVENT_H
