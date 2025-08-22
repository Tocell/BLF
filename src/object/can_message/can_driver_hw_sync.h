#ifndef CAN_DRIVER_HW_SYNC_H
#define CAN_DRIVER_HW_SYNC_H

#include "../common.h"
#include "object_header.h"

#define CAN_DRIVER_ERROR_HW_SYNC_FLAGS_TX       0x01
#define CAN_DRIVER_ERROR_HW_SYNC_FLAGS_RX       0x02
#define CAN_DRIVER_ERROR_HW_SYNC_FLAGS_TXTHIS   0x04

BLFSTART
class TWBLF_API CanDriverHWSync final : public ObjectHeader
{
public:
    uint16_t channel {};
    uint8_t flags {};
    uint8_t reserved1 {};
    uint32_t reserved2 {};

    [[nodiscard]] uint32_t can_driver_hw_sync_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(flags) +
               sizeof(reserved1) +
               sizeof(reserved2);
    }
};
BLFEND

#endif //CAN_DRIVER_HW_SYNC_H
