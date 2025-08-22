#ifndef FLEXRAY_V6_START_CYCLE_EVENT_H
#define FLEXRAY_V6_START_CYCLE_EVENT_H

#include "../common.h"
#include "object_header.h"
#include <array>

BLFSTART
class TWBLF_API FlexRayV6StartCycleEvent : public ObjectHeader
{
public:
    uint16_t channel {};
    uint8_t dir {};                                 /* dir flag (tx, rx) */
    uint8_t low_time {};
    uint32_t fpga_tick {};
    uint32_t fpga_tick_overflow {};
    uint32_t v6_start_cycle_event_client_idx {};
    uint32_t cluster_time {};
    std::array<uint8_t, 2> data_bytes {};
    uint16_t reserved {};

    [[nodiscard]] uint32_t flexray_v6_start_cycle_event_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(dir) +
               sizeof(low_time) +
               sizeof(fpga_tick) +
               sizeof(fpga_tick_overflow) +
               sizeof(v6_start_cycle_event_client_idx) +
               sizeof(cluster_time) +
               sizeof(data_bytes) +
               sizeof(reserved);
    }
};
BLFEND

#endif //FLEXRAY_V6_START_CYCLE_EVENT_H
