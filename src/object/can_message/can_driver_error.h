#ifndef CAN_DRIVER_ERROR_H
#define CAN_DRIVER_ERROR_H
#include "../common.h"
#include "object_header.h"

BLFSTART
class TWBLF_API CanDriverError final : public ObjectHeader
{
public:
    uint16_t channel {};
    uint8_t tx_error {};
    uint8_t rx_error {};
    uint32_t error_code {};

    [[nodiscard]] uint32_t can_driver_error_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(tx_error) +
               sizeof(rx_error) +
               sizeof(error_code);
    }
};
BLFEND
#endif //CAN_DRIVER_ERROR_H
