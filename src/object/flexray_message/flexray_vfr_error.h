#ifndef FLEXRAY_VFR_ERROR_H
#define FLEXRAY_VFR_ERROR_H

#include "../common.h"
#include "object_header.h"
#include <array>

BLFSTART
class TWBLF_API FlexrayVFrError : public ObjectHeader
{
public:
    uint16_t channel {};
    uint16_t version {};
    uint16_t channel_mask {};
    uint8_t cycle {};
    uint32_t vfr_error_client_index {};
    uint32_t cluster_no {};
    uint32_t tag {};
    std::array<uint32_t, 4> data {};
    uint16_t reserved2 {};

    [[nodiscard]] uint32_t flexray_vfr_error_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(version) +
               sizeof(channel_mask) +
               sizeof(cycle) +
               sizeof(vfr_error_client_index) +
               sizeof(cluster_no) +
               sizeof(tag) +
               sizeof(data) +
               sizeof(reserved2);
    }
};
BLFEND

#endif //FLEXRAY_VFR_ERROR_H
