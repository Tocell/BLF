#ifndef CANMESSAGE_H
#define CANMESSAGE_H
#include "../common.h"
#include "object/object_header/object_header.h"
#include <array>
#include <cstdint>

#define CAN_MESSAGE_FLAG_TX         0x01
#define CAN_MESSAGE_FLAG_RX         0x00

BLFSTART
class TWBLF_API CanMessage : public ObjectHeader
{
public:
    uint16_t channel {};
    /*
     * bit[0]: tx:1  rx:0
     */
    uint8_t flags {};
    uint8_t dlc {};
    uint32_t id {};
    std::array<uint8_t, 8> data {};

    [[nodiscard]] uint32_t can_message_object_size() const {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(flags) +
               sizeof(dlc) +
               sizeof(id) +
               BL_DLC_TO_FRAME_LENGTH(this->dlc);
    }
};
BLFEND

#endif
