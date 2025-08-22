#ifndef LIN_SCHEDULEDR_MODE_CHANGE_H
#define LIN_SCHEDULEDR_MODE_CHANGE_H

#include "../common.h"
#include "object_header.h"

BLFSTART
class TWBLF_API LinSchedulerModeChange : public ObjectHeader
{
public:
    uint16_t channel {};
    uint8_t old_mode {};
    uint8_t new_mode {};
    uint8_t old_slot {};
    uint8_t new_slot {};
    uint8_t first_event_after_wakeup {};

    [[nodiscard]] uint32_t lin_scheduler_mode_change_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(old_mode) +
               sizeof(new_mode) +
               sizeof(old_slot) +
               sizeof(new_slot) +
               sizeof(first_event_after_wakeup);
    }
};
BLFEND


#endif //LIN_SCHEDULEDR_MODE_CHANGE_H
