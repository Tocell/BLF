#ifndef CAN_ERROR_FRAME_H
#define CAN_ERROR_FRAME_H

#include "../common.h"
#include "object_header.h"

BLFSTART
class TWBLF_API CanErrorFrame final : public ObjectHeader
{
public:
    uint16_t channel {};
    uint16_t length {};

    [[nodiscard]] uint32_t can_error_frame_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(length);
    }
};
BLFEND

#endif //CAN_ERROR_FRAME_H
