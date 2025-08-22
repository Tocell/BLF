#ifndef LIN_SPIKE_EVENT2_H
#define LIN_SPIKE_EVENT2_H

#include "../common.h"
#include "object_header.h"
#include "lin_bus_event.h"
#include <array>

BLFSTART
class TWBLF_API LinSpikeEvent2 : public ObjectHeader
{
public:
    LinBusEvent lin_bus_event;
    uint32_t width {};
    uint8_t internal {};
    std::array<uint8_t, 3> reserved {};

    [[nodiscard]] uint32_t lin_spike_event2_object_size() const
    {
        return object_header_object_size() +
               lin_bus_event.lin_bus_event_object_size() +
               sizeof(width) +
               sizeof(internal) +
               sizeof(reserved);
    }
};
BLFEND

#endif //LIN_SPIKE_EVENT2_H
