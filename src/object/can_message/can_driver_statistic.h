#ifndef CAN_DRIVER_STATISTIC_H
#define CAN_DRIVER_STATISTIC_H

#include "../common.h"
#include "object_header.h"

BLFSTART
class TWBLF_API CanDriverStatistic final : public ObjectHeader
{
public:
    uint16_t channel {};
    uint16_t busload {};
    uint32_t standard_data_frames {};
    uint32_t extended_data_frames {};
    uint32_t standard_remote_frames {};
    uint32_t extended_remote_frames {};
    uint32_t error_frames {};
    uint32_t overload_frames {};
    uint32_t reserved {};

    [[nodiscard]] uint32_t can_driver_statistic_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(busload) +
               sizeof(standard_data_frames) +
               sizeof(extended_data_frames) +
               sizeof(standard_remote_frames) +
               sizeof(extended_remote_frames) +
               sizeof(error_frames) +
               sizeof(overload_frames) +
               sizeof(reserved);
    }
};
BLFEND

#endif //CAN_DRIVER_STATISTIC_H
