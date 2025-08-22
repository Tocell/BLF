#ifndef LIN_LONG_DOM_SIGNAL_EVENT2_H
#define LIN_LONG_DOM_SIGNAL_EVENT2_H

#include "../common.h"
#include "object_header.h"
#include "lin_bus_event.h"
#include <array>

BLFSTART
class TWBLF_API LinLongDomSignalEvent2 : public ObjectHeader
{
public:
    LinBusEvent lin_bus_event {};
    uint8_t type {};
    std::array<uint8_t, 7> reserved{};
    uint64_t length {};

    [[nodiscard]] uint32_t lin_long_dom_signal_event2_object_size() const
    {
        return object_header_object_size() +
               lin_bus_event.lin_bus_event_object_size() +
               sizeof(type) +
               sizeof(reserved) +
               sizeof(length);
    }
};
BLFEND

#endif //LIN_LONG_DOM_SIGNAL_EVENT2_H
