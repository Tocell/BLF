#ifndef LIN_DLC_INFO_H
#define LIN_DLC_INFO_H

#include "../common.h"
#include "object_header.h"
#include <array>

BLFSTART
class TWBLF_API LinDlcInfo : public ObjectHeader
{
public:
    uint16_t channel {};
    uint8_t id {};
    uint8_t dlc {};

    [[nodiscard]] uint32_t lin_dlc_info_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(id) +
               sizeof(dlc);
    }
};
BLFEND

#endif //LIN_DLC_INFO_H
