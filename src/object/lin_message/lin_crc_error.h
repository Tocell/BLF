#ifndef LIN_CRC_ERROR_H
#define LIN_CRC_ERROR_H

#include "../common.h"
#include "object_header.h"
#include <array>

BLFSTART
class TWBLF_API LinCrcError : public ObjectHeader
{
public:
    uint16_t channel {};
    uint8_t id {};
    uint8_t dlc {};
    std::array<uint8_t, 8> data {};
    uint8_t fsm_id {};
    uint8_t fsm_state {};
    uint8_t header_time {};
    uint8_t full_time {};
    uint16_t crc {};
    uint8_t dir {};
    uint8_t reserved {};

    [[nodiscard]] uint32_t lin_crc_error_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(id) +
               sizeof(dlc) +
               sizeof(data) +
               sizeof(fsm_id) +
               sizeof(fsm_state) +
               sizeof(header_time) +
               sizeof(full_time) +
               sizeof(crc) +
               sizeof(dir) +
               sizeof(reserved);
    }
};
BLFEND

#endif //LIN_CRC_ERROR_H
