#ifndef LIN_SPIKE_EVENT_H
#define LIN_SPIKE_EVENT_H

#include "../common.h"
#include "object_header.h"

BLFSTART
class TWBLF_API LinSpikeEvent : public ObjectHeader
{
public:
    uint16_t channel {};
    uint32_t width {};

    [[nodiscard]] uint32_t lin_spike_event_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(width);
    }
};
BLFEND

#endif //LIN_SPIKE_EVENT_H
