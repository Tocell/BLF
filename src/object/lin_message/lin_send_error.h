#ifndef LIN_SEND_ERROR_H
#define LIN_SEND_ERROR_H

#include "../common.h"
#include "object_header.h"

BLFSTART
class TWBLF_API LinSendError : public ObjectHeader
{
public:
    uint16_t channel {};
    uint8_t id {};
    uint8_t dlc {};
    uint8_t fsm_id {};
    uint8_t fsm_state {};
    uint8_t header_time {};
    uint8_t full_time {};

    [[nodiscard]] uint32_t lin_send_error_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(id) +
               sizeof(dlc) +
               sizeof(fsm_id) +
               sizeof(fsm_state) +
               sizeof(header_time) +
               sizeof(full_time);
    }
};
BLFEND

#endif //LIN_SEND_ERROR_H
