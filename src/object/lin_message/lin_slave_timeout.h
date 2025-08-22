#ifndef LIN_SLAVE_TIMEOUT_H
#define LIN_SLAVE_TIMEOUT_H

#include "../common.h"
#include "object_header.h"

BLFSTART
class TWBLF_API LinSlaveTimeout : public ObjectHeader
{
public:
    uint16_t channel {};
    uint8_t slave_id {};
    uint8_t state_id {};
    uint32_t follow_state_id {};

    [[nodiscard]] uint32_t lin_slave_timeout_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(slave_id) +
               sizeof(state_id) +
               sizeof(follow_state_id);
    }
};
BLFEND

#endif //LIN_SLAVE_TIMEOUT_H
