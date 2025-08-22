#ifndef FLEXRAY_VFR_STATUS_H
#define FLEXRAY_VFR_STATUS_H

#include "../common.h"
#include "object_header.h"
#include <array>

BLFSTART
class TWBLF_API FlexrayVFrStatus : public ObjectHeader
{
public:
    uint16_t channel {};
    uint16_t version {};
    uint16_t channel_mask {};
    uint8_t cycle {};
    uint32_t vfr_start_client_index {};
    uint32_t cluster_no {};
    uint32_t wus {};
    uint32_t cc_sync_state {};
    uint32_t tag {};
    std::array<uint32_t, 2> data {};
    std::array<uint16_t, 16> reserved {};

    [[nodiscard]] uint32_t flexray_vfr_status_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(version) +
               sizeof(channel_mask) +
               sizeof(cycle) +
               sizeof(vfr_start_client_index) +
               sizeof(cluster_no) +
               sizeof(wus) +
               sizeof(cc_sync_state) +
               sizeof(tag) +
               sizeof(data) +
               sizeof(reserved);
    }
};
BLFEND

#endif //FLEXRAY_VFR_STATUS_H
