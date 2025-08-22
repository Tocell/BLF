#ifndef LIN_STATISTIC_EVENT_H
#define LIN_STATISTIC_EVENT_H

#include "../common.h"
#include "object_header.h"

BLFSTART
class TWBLF_API LinStatisticEvent : public ObjectHeader
{
public:
    uint16_t channel {};
    double busLoad {};
    uint32_t bursts_total {};
    uint32_t bursts_overrun {};
    uint32_t frames_sent {};
    uint32_t frames_received {};
    uint32_t frames_unanswered {};

    [[nodiscard]] uint32_t lin_statistic_event_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(busLoad) +
               sizeof(bursts_total) +
               sizeof(bursts_overrun) +
               sizeof(frames_sent) +
               sizeof(frames_received) +
               sizeof(frames_unanswered);
    }
};
BLFEND

#endif //LIN_STATISTIC_EVENT_H
