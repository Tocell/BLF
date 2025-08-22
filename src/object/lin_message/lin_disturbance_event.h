#ifndef LIN_DISTURBANCE_EVENT_H
#define LIN_DISTURBANCE_EVENT_H

#include "../common.h"
#include "object_header.h"
#include <array>

BLFSTART
class TWBLF_API LinDisturbanceEvent : public ObjectHeader
{
public:
    uint16_t channel {};
    uint8_t id {};
    uint8_t disturbing_frame_id {};
    uint32_t disturbance_type {};
    uint32_t byte_index {};
    uint32_t bit_index {};
    uint32_t bit_offset_in_sixteenth_bits {};
    uint32_t disturbance_length_in_sixteenth_bits {};

    [[nodiscard]] uint32_t lin_disturbance_event_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(id) +
               sizeof(disturbing_frame_id) +
               sizeof(disturbance_type) +
               sizeof(byte_index) +
               sizeof(bit_index) +
               sizeof(bit_offset_in_sixteenth_bits) +
               sizeof(disturbance_length_in_sixteenth_bits);
    }
};
BLFEND

#endif //LIN_DISTURBANCE_EVENT_H
