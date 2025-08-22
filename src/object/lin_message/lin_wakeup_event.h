#ifndef LIN_WAKEUP_EVENT_H
#define LIN_WAKEUP_EVENT_H

#include "../common.h"
#include "object_header.h"

BLFSTART
    class TWBLF_API LinWakeupEvent : public ObjectHeader
{
public:
    uint16_t channel {};
    uint8_t signal {};
    uint8_t external {};

    [[nodiscard]] uint32_t lin_wakeup_event_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(signal) +
               sizeof(external);
    }
};
BLFEND

#endif //LIN_WAKEUP_EVENT_H
