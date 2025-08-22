#ifndef LIN_SLEEP_MODE_EVENT_H
#define LIN_SLEEP_MODE_EVENT_H

#include "../common.h"
#include "object_header.h"

#define LIN_SLEEP_MODE_EVENT_FLAG_WAS_AWAKE     0x01
#define LIN_SLEEP_MODE_EVENT_FLAG_IS_AWAKE      0x02
#define LIN_SLEEP_MODE_EVENT_FLAG_EXTERNAL      0x04

BLFSTART
class TWBLF_API LinSleepModeEvent : public ObjectHeader
{
public:
    uint16_t channel {};
    uint8_t reason {};
    uint8_t flags {};

    [[nodiscard]] uint32_t lin_sleep_mode_event_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(reason) +
               sizeof(flags);
    }
};
BLFEND

#endif //LIN_SLEEP_MODE_EVENT_H
