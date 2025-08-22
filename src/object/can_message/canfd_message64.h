#ifndef CANFD_MESSAGE64_H
#define CANFD_MESSAGE64_H

#include "../common.h"
#include "object_header.h"
#include "canfd_ext_frame_data.h"
/*
* - Bit#=12 (0x1000): EDL, 0: CAN frame, 1: CAN FD frame
* - Bit#=13 (0x2000): BRS (CAN FD)
* - Bit#=14 (0x4000): ESI
* */
#define CANFD_MESSAGE64_FLAGS_EDL  0x00001000
#define CANFD_MESSAGE64_FLAGS_BRS  0x00002000
#define CANFD_MESSAGE64_FLAGS_ESI  0x00004000

BLFSTART
class TWBLF_API CanFdMessage64 final : public ObjectHeader
{
public:
    uint8_t channel {0};
    uint8_t dlc {0};
    uint8_t valid_data_bytes {0};    /* Valid payload length of mData */
    uint8_t tx_count {0};
    uint32_t id {0};
    uint32_t frame_length {0};   /* message length in ns - without 3 inter frame space bits */

    uint32_t flags {0x00000000};
    uint32_t btr_cfg_arb {0};
    uint32_t btr_cfg_data {0};
    uint32_t time_offset_brs_ns {0};
    uint32_t time_offset_crc_del_ns {0};
    uint16_t bit_count {0};
    uint8_t dir {0};
    uint8_t ext_data_offset {0};    // have ext_frame_data ?
    uint32_t crc {0};
    std::array<uint8_t, 64> data {};

    CanFdExtFrameData ext_frame_data {};

    [[nodiscard]] uint32_t canfd_message64_object_size() const
    {
        uint32_t size = object_header_object_size() +
                        sizeof(channel) +
                        sizeof(dlc) +
                        sizeof(valid_data_bytes) +
                        sizeof(tx_count) +
                        sizeof(id) +
                        sizeof(frame_length) +
                        sizeof(flags) +
                        sizeof(btr_cfg_arb) +
                        sizeof(btr_cfg_data) +
                        sizeof(time_offset_brs_ns) +
                        sizeof(time_offset_crc_del_ns) +
                        sizeof(bit_count) +
                        sizeof(dir) +
                        sizeof(ext_data_offset) +
                        sizeof(crc) +
                        BL_DLC_TO_FRAME_LENGTH(this->dlc) * sizeof(uint8_t);

        if (ext_data_offset) {
            size += ext_frame_data.canfd_ext_frame_data_object_size();
        }
        return size;
    }
};
BLFEND

#endif //CANFD_MESSAGE64_H
