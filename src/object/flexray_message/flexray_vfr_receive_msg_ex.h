#ifndef FLEXRAY_VFR_RECEIVE_MSG_EX_H
#define FLEXRAY_VFR_RECEIVE_MSG_EX_H

#include "../common.h"
#include "object_header.h"
#include <array>

BLFSTART
class TWBLF_API FlexrayVFrReceiveMsgEx : public ObjectHeader
{
public:
    uint16_t channel {};
    uint16_t version {};
    uint16_t channel_mask {};
    uint16_t dir {};
    uint32_t vfr_recv_msg_ex_client_index {};
    uint32_t cluster_no {};
    uint16_t frame_id {};
    uint16_t header_crc1 {};
    uint16_t header_crc2 {};
    uint16_t byte_count {};
    uint16_t data_count {};         /* length of the data array (stretchy struct) */
    uint16_t cycle {};
    uint32_t tag {};
    uint32_t data {};
    uint32_t frame_flags {};
    uint32_t app_parameter {};
    uint32_t frame_crc {};
    uint32_t frame_length_ns {};
    uint16_t frame_id1 {};
    uint16_t pdu_offset {};
    uint16_t blf_log_mask {};
    uint16_t reserved_w {};
    std::array<uint32_t, 6> reserved{};
    std::array<uint8_t, 254> data_bytes {};

    [[nodiscard]] uint32_t flexray_vfr_receive_msg_ex_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(version) +
               sizeof(channel_mask) +
               sizeof(dir) +
               sizeof(vfr_recv_msg_ex_client_index) +
               sizeof(cluster_no) +
               sizeof(frame_id) +
               sizeof(header_crc1) +
               sizeof(header_crc2) +
               sizeof(byte_count) +
               sizeof(data_count) +
               sizeof(cycle) +
               sizeof(tag) +
               sizeof(data) +
               sizeof(frame_flags) +
               sizeof(app_parameter) +
               sizeof(frame_crc) +
               sizeof(frame_length_ns) +
               sizeof(frame_id1) +
               sizeof(pdu_offset) +
               sizeof(blf_log_mask) +
               sizeof(reserved_w) +
               sizeof(reserved) +
               sizeof(data_bytes);
    }
};
BLFEND

#endif //FLEXRAY_VFR_RECEIVE_MSG_EX_H
