#ifndef LIN_SYNCH_FIELD_EVENT_H
#define LIN_SYNCH_FIELD_EVENT_H

#include "../common.h"
#include "object_header.h"
#include <array>

#include "lin_bus_event.h"

BLFSTART
    class TWBLF_API LinSynchFieldEvent : public LinBusEvent
{
public:
    uint64_t synch_break_length {};
    uint64_t synch_del_length {};

    [[nodiscard]] uint32_t lin_synch_field_event_object_size() const
    {
        return lin_bus_event_object_size() +
               sizeof(synch_break_length) +
               sizeof(synch_del_length);
    }
};
BLFEND

#endif //LIN_SYNCH_FIELD_EVENT_H
