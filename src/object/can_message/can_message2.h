#ifndef CAN_MESSAGE2_H
#define CAN_MESSAGE2_H

#include "../common.h"
#include "object_header.h"
#include <array>

BLFSTART
class TWBLF_API CanMessage2 final : public ObjectHeader
{
public:
    uint16_t channel {};
    uint8_t flags {};
    uint8_t dlc {};
    uint32_t id {};
    std::array<uint8_t, 8> data {};
    uint32_t frame_length {};
    uint8_t bit_count {};
    uint8_t reserved_canmessage1 {};
    uint16_t reserved_canmessage2 {};

    [[nodiscard]] uint32_t can_message2_object_first_parted_size() const
    {
        return sizeof(channel) +
               sizeof(flags) +
               sizeof(dlc) +
               sizeof(id) +
               BL_DLC_TO_FRAME_LENGTH(this->dlc) * sizeof(uint8_t);
    }

    [[nodiscard]] uint32_t can_message2_object_second_parted_size() const
    {
        return sizeof(frame_length) +
               sizeof(bit_count) +
               sizeof(reserved_canmessage1) +
               sizeof(reserved_canmessage2);
    }
};
BLFEND

#endif //CAN_MESSAGE2_H
