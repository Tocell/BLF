#ifndef CAN_OVERLOAD_FRAME_H
#define CAN_OVERLOAD_FRAME_H

#include "../common.h"
#include "object_header.h"

BLFSTART
class TWBLF_API CanOverloadFrame final : public ObjectHeader
{
public:
    uint16_t channel {};
    uint16_t reserved1 {};
    uint32_t reserved2 {};

    [[nodiscard]] uint32_t can_overload_frame_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(reserved1) +
               sizeof(reserved2);
    }
};
BLFEND

#endif //CAN_OVERLOAD_FRAME_H
