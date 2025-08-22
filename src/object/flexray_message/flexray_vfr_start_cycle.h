#ifndef FLEXRAY_VFR_START_CYCLE_H
#define FLEXRAY_VFR_START_CYCLE_H

#include "../common.h"
#include "object_header.h"
#include <array>

BLFSTART
class TWBLF_API FlexrayVFrStartCycle : public ObjectHeader
{
public:
    uint16_t channel {};
    uint16_t version {};
    uint16_t channel_mask {};
    uint8_t dir {};
    uint8_t cycle {};
    uint32_t vfr_start_cycle_client_index {};
    uint32_t cluster_no {};
    uint16_t nm_size {};
    std::array<uint8_t, 12> data_bytes {};
    uint32_t tag {};
    std::array<uint32_t, 5> data {};
    uint16_t reserved {};

    [[nodiscard]] uint32_t flexray_vfr_start_cycle_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(version) +
               sizeof(channel_mask) +
               sizeof(dir) +
               sizeof(cycle) +
               sizeof(vfr_start_cycle_client_index) +
               sizeof(cluster_no) +
               sizeof(nm_size) +
               sizeof(data_bytes) +
               sizeof(tag) +
               sizeof(data) +
               sizeof(reserved);
    }
};
BLFEND

#endif //FLEXRAY_VFR_START_CYCLE_H
