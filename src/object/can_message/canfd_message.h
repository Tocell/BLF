#ifndef CANFD_MESSAGE_H
#define CANFD_MESSAGE_H

#include "../common.h"
#include "object_header.h"
#include <array>

// flags
#define CANFD_MESSAGE_FLAG_TX         0x00000001
#define CANFD_MESSAGE_FLAG_RX         0x00000000
#define CANFD_MESSAGE_FLAG_NERR       0x00000020
#define CANFD_MESSAGE_FLAG_WU         0x00000040
#define CANFD_MESSAGE_FLAG_RTR        0x00000080

// canfd_flags
#define CANFD_MESSAGE_CANFD_FLAGS_EDL   0x01    // canfd flag
#define CANFD_MESSAGE_CANFD_FLAGS_BRS   0x02    // speed change flag
#define CANFD_MESSAGE_CANFD_FLAGS_ESI   0x04    // frame error flag

BLFSTART
class TWBLF_API CanFdMessage final : public ObjectHeader
{
public:
    uint16_t channel {};
    /*
     * bit[0]: tx:1  rx:0
     */
    uint8_t flags {0x00};               /* CAN dir & rtr */
    uint8_t dlc {};
    uint32_t id {};
    uint32_t frame_length {};
    uint8_t arb_bit_count {};
    uint8_t canfd_flags {};             /* CAN FD flags */
    uint8_t valid_data_bytes {};
    uint8_t reserved1 {};
    uint32_t reserved2 {};
    std::array<uint8_t, 64> data {};
    uint32_t reserved3 {};

    [[nodiscard]] uint32_t canfd_message_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(flags) +
               sizeof(dlc) +
               sizeof(id) +
               sizeof(frame_length) +
               sizeof(arb_bit_count) +
               sizeof(canfd_flags) +
               sizeof(valid_data_bytes) +
               sizeof(reserved1) +
               sizeof(reserved2) +
               BL_DLC_TO_FRAME_LENGTH(this->dlc) +
               sizeof(reserved3);
    }
};
BLFEND

#endif //CANFD_MESSAGE_H
