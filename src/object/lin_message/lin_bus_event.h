#ifndef LIN_BUS_EVENT_H
#define LIN_BUS_EVENT_H

#include "../common.h"
#include "object_header.h"
#include <array>

BLFSTART
class TWBLF_API LinBusEvent
{
public:
    uint64_t sof {};
    uint32_t event_baudrate {};
    uint16_t channel {};
    std::array<uint8_t, 2> reserved {};

    [[nodiscard]] uint32_t lin_bus_event_object_size() const
    {
        return sizeof(sof) +
               sizeof(event_baudrate) +
               sizeof(channel) +
               sizeof(reserved);
    }
};
BLFEND

#endif //LIN_BUS_EVENT_H
