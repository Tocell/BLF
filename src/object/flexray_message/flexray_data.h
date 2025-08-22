#ifndef FLEXRAY_DATA_H
#define FLEXRAY_DATA_H

#include "../common.h"
#include "object_header.h"
#include <array>

BLFSTART
class TWBLF_API FlexrayData : public ObjectHeader
{
public:
    uint16_t channel {};
    uint8_t mux {};
    uint8_t len {};
    uint16_t message_id {};
    uint16_t crc {};
    uint8_t dir {};
    uint8_t dummy1 {};
    uint16_t dummy2 {};
    std::array<uint8_t, 12> data_bytes {};

    [[nodiscard]] uint32_t flexray_data_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(mux) +
               sizeof(len) +
               sizeof(message_id) +
               sizeof(crc) +
               sizeof(dir) +
               sizeof(dummy1) +
               sizeof(dummy2) +
               sizeof(data_bytes);
    }
};
BLFEND

#endif //FLEXRAY_DATA_H
