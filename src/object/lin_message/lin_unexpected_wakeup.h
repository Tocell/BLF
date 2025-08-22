#ifndef LIN_UNEXPECTED_WAKEUP_H
#define LIN_UNEXPECTED_WAKEUP_H

#include "../common.h"
#include "object_header.h"
#include <array>
#include "lin_bus_event.h"

BLFSTART
    class TWBLF_API LinUnexpectedWakeup : public ObjectHeader
{
public:
    LinBusEvent lin_bus_event;
    uint64_t width {};
    uint8_t signal {};
    std::array<uint8_t, 7> reserved {};

    [[nodiscard]] uint32_t lin_unexpected_wakeup_object_size() const
    {
        return object_header_object_size() +
               lin_bus_event.lin_bus_event_object_size() +
               sizeof(width) +
               sizeof(signal) +
               sizeof(reserved);
    }
};
BLFEND

#endif //LIN_UNEXPECTED_WAKEUP_H
