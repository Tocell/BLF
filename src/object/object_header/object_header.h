#ifndef OBJECTHEADER_H
#define OBJECTHEADER_H

#include <cstdint>

#include "object_header_base.h"

BLFSTART
class TWBLF_API ObjectHeader : public ObjectHeaderBase
{
public:
    uint32_t object_flags {BL_OBJ_FLAG_TIME_ONE_NANS};
    uint16_t client_index {};
    uint16_t object_version {0};
    uint64_t object_timestamp {};

    [[nodiscard]] uint32_t object_header_header_size() const
    {
        return object_header_base_header_size() + sizeof(object_flags) + sizeof(client_index) + sizeof(object_version) + sizeof(object_timestamp);
    }

    [[nodiscard]] uint32_t object_header_object_size() const
    {
        return object_header_base_object_size() + sizeof(object_flags) + sizeof(client_index) + sizeof(object_version) + sizeof(object_timestamp);
    }
};

BLFEND
#endif