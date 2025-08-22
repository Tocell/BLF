#ifndef CAN_DRIVER_ERROR_EXT_H
#define CAN_DRIVER_ERROR_EXT_H

#include "../common.h"
#include "object_header.h"
#include <array>

BLFSTART
class TWBLF_API CanDriverErrorExt final : public ObjectHeader
{
public:
    uint16_t channel {};
    uint8_t tx_errors {};
    uint8_t rx_errors {};
    uint32_t error_code {};
    uint8_t state{};
    uint8_t reserved1{};
    uint8_t reserved2{};
    std::array<uint32_t, 4> reserved3{};

    [[nodiscard]] uint32_t can_driver_error_ext_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(tx_errors) +
               sizeof(rx_errors) +
               sizeof(error_code) +
               sizeof(state) +
               sizeof(reserved1) +
               sizeof(reserved2) +
               sizeof(reserved3);
    }
};
BLFEND

#endif //CAN_DRIVER_ERROR_EXT_H
