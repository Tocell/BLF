#ifndef LIN_RECEIVE_ERROR_H
#define LIN_RECEIVE_ERROR_H

#include "../common.h"
#include "object_header.h"
#include <array>

BLFSTART
class TWBLF_API LinReceiveError : public ObjectHeader
{
public:
    uint16_t channel {};
    uint8_t id {};
    uint8_t dlc {};
    uint8_t fsm_id {};
    uint8_t fsm_state {};
    uint8_t header_time {};
    uint8_t full_time {};
    uint8_t state_reason {};
    uint8_t offending_byte {};
    uint8_t short_error {};
    uint8_t timeout_during_dlc_detection {};

    [[nodiscard]] uint32_t lin_receive_error_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(id) +
               sizeof(dlc) +
               sizeof(fsm_id) +
               sizeof(fsm_state) +
               sizeof(header_time) +
               sizeof(full_time) +
               sizeof(state_reason) +
               sizeof(offending_byte) +
               sizeof(short_error) +
               sizeof(timeout_during_dlc_detection);
    }
};
BLFEND

#endif //LIN_RECEIVE_ERROR_H
