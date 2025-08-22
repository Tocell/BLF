#ifndef FLEXRAY_VFR_RECEIVE_MSG_H
#define FLEXRAY_VFR_RECEIVE_MSG_H

#include "../common.h"
#include "object_header.h"
#include <array>

BLFSTART
class TWBLF_API FlexrayVFrReceiveMsg : public ObjectHeader
{
public:
    uint16_t channel {};
    uint16_t version {};
    uint16_t channelMask {};
    uint8_t dir {};                     /* dir flag (tx, rx) */
    uint32_t vfr_recv_client_index {};
    uint32_t cluster_no {};
    uint16_t frame_id {};
    uint16_t header_crc1 {};
    uint16_t header_crc2 {};
    uint16_t byte_count {};
    uint16_t data_count {};
    uint8_t cycle {};
    uint32_t tag {};
    uint32_t data {};
    uint32_t frame_flags {};
    uint32_t app_parameter {};
    std::array<uint8_t, 254> data_bytes {};

    [[nodiscard]] uint32_t flexray_vfr_receive_msg_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(version) +
               sizeof(channelMask) +
               sizeof(dir) +
               sizeof(vfr_recv_client_index) +
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
               sizeof(data_bytes);
    }
};
BLFEND

#endif //FLEXRAY_VFR_RECEIVE_MSG_H
