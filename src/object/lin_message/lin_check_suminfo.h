#ifndef LIN_CHECK_SUMINFO_H
#define LIN_CHECK_SUMINFO_H

#include "../common.h"
#include "object_header.h"
#include <array>

BLFSTART
class TWBLF_API LinChecksumInfo : public ObjectHeader
{
public:
    uint16_t channel {};
    uint8_t id {};
    uint8_t checksum_model {};

    [[nodiscard]] uint32_t lin_checksum_info_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(id) +
               sizeof(checksum_model);
    }
};
BLFEND

#endif //LIN_CHECK_SUMINFO_H
