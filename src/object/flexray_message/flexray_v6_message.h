#ifndef FLEXRAY_V6_MESSAGE_H
#define FLEXRAY_V6_MESSAGE_H

#include "../common.h"
#include "object_header.h"
#include <array>

BLFSTART
class TWBLF_API FlexrayV6Message : public ObjectHeader
{
public:
    uint16_t channel {};
    uint8_t dir {};                     /* dir flag (tx, rx) */
    uint8_t low_time {};                /* additional time field in simulation */
    uint32_t fpga_tick {};              /* timestamp generated from xModule */
    uint32_t fpga_tick_overflow {};     /* overflow counter of the timestamp */
    uint32_t v6_msg_client_index {};     /* clientindex of send node */
    uint32_t cluster_time {};           /* relatvie clustertime, from 0 to cyclelength*/
    uint16_t frame_id {};
    uint16_t header_crc {};
    uint16_t frame_state {};
    uint8_t length {};                  /* dlc of message,  byte  */
    uint8_t cycle {};
    uint8_t header_bit_mask {};         /* Bit0 = NMBit, Bit1 = SyncBit, Bit2 = Reserved */
    uint8_t reserved1 {};
    uint16_t reserved2 {};
    std::array<uint8_t, 64> data_bytes {};

    [[nodiscard]] uint32_t flexray_v6_message_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(dir) +
               sizeof(low_time) +
               sizeof(fpga_tick) +
               sizeof(fpga_tick_overflow) +
               sizeof(v6_msg_client_index) +
               sizeof(cluster_time) +
               sizeof(frame_id) +
               sizeof(header_crc) +
               sizeof(frame_state) +
               sizeof(length) +
               sizeof(cycle) +
               sizeof(header_bit_mask) +
               sizeof(reserved1) +
               sizeof(reserved2) +
               sizeof(data_bytes);
    }
};
BLFEND

#endif //FLEXRAY_V6_MESSAGE_H
