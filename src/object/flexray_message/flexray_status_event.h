#ifndef FLEXRAY_STATUS_EVENT_H
#define FLEXRAY_STATUS_EVENT_H

#include "../common.h"
#include "object_header.h"
#include <array>

BLFSTART
class TWBLF_API FlexrayStatusEvent : public ObjectHeader
{
public:
    uint16_t channel {};
    uint16_t version {};
    uint16_t status_type {};
    uint16_t info_mask1 {};
    uint16_t info_mask2 {};
    uint16_t info_mask3 {};
    std::array<uint16_t, 16> reserved {};

    [[nodiscard]] uint32_t flexray_status_event_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(version) +
               sizeof(status_type) +
               sizeof(info_mask1) +
               sizeof(info_mask2) +
               sizeof(info_mask3) +
               sizeof(reserved);
    }
};
BLFEND

#endif //FLEXRAY_STATUS_EVENT_H
