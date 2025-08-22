#ifndef CAN_ERROR_FRAME_EXT_H
#define CAN_ERROR_FRAME_EXT_H

#include "../common.h"
#include "object_header.h"

BLFSTART
class TWBLF_API CanErrorFrameExt final : public ObjectHeader
{
public:
    uint16_t channel {};
    uint16_t length {};
    uint32_t flags {};
    uint8_t ecc {};
    uint8_t position {};
    uint8_t dlc {};
    uint8_t reserved1 {};
    uint32_t frame_length_in_ns {};
    uint32_t id {};
    uint16_t flags_ext {};
    uint16_t reserved2 {};
    std::array<uint8_t, 8> data {};

    [[nodiscard]] uint32_t can_error_frame_ext_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(length) +
               sizeof(flags) +
               sizeof(ecc) +
               sizeof(position) +
               sizeof(dlc) +
               sizeof(reserved1) +
               sizeof(frame_length_in_ns) +
               sizeof(id) +
               sizeof(flags_ext) +
               sizeof(reserved2) +
               BL_DLC_TO_FRAME_LENGTH(this->dlc);
    }
};
BLFEND

#endif //CAN_ERROR_FRAME_EXT_H
