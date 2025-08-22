#ifndef LIN_SYNC_ERROR2_H
#define LIN_SYNC_ERROR2_H

#include "../common.h"
#include "object_header.h"
#include <array>

#include "lin_synch_field_event.h"

BLFSTART
    class TWBLF_API LinSyncError2 : public ObjectHeader
{
public:
    LinSynchFieldEvent lin_synch_field_event;
    std::array<uint16_t, 4> time_diff {};

    [[nodiscard]] uint32_t lin_sync_error2_object_size() const
    {
        return object_header_object_size() +
               lin_synch_field_event.lin_synch_field_event_object_size() +
               sizeof(time_diff);
    }
};
BLFEND

#endif //LIN_SYNC_ERROR2_H
