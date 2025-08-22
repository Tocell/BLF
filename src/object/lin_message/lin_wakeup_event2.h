#ifndef LIN_WAKEUP_EVENT2_H
#define LIN_WAKEUP_EVENT2_H

#include "../common.h"
#include "object_header.h"
#include "lin_bus_event.h"

BLFSTART
    class TWBLF_API LinWakeupEvent2 : public ObjectHeader
{
public:
    LinBusEvent lin_bus_event;
    uint8_t length_info {};
    uint8_t signal {};
    uint8_t external {};
    uint8_t reserved {};

    [[nodiscard]] uint32_t lin_wakeup_event2_object_size() const
    {
        return object_header_object_size() +
               lin_bus_event.lin_bus_event_object_size() +
               sizeof(length_info) +
               sizeof(signal) +
               sizeof(external) +
               sizeof(reserved);
    }
};
BLFEND

#endif //LIN_WAKEUP_EVENT2_H
