#ifndef LIN_SYNC_ERROR_H
#define LIN_SYNC_ERROR_H

#include "../common.h"
#include "object_header.h"
#include <array>

BLFSTART
class TWBLF_API LinSyncError : public ObjectHeader
{
public:
    uint16_t channel {};
    uint16_t dummy;
    std::array<uint16_t, 4> time_diff {};

    [[nodiscard]] uint32_t lin_sync_error_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(dummy) +
               sizeof(time_diff);
    }
};
BLFEND

#endif //LIN_SYNC_ERROR_H
