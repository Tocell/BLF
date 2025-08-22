#ifndef OBJECTHEADERBASE_H
#define OBJECTHEADERBASE_H

#include <cstdint>
#include "common.h"

#define BL_OBJ_FLAG_TIME_TEN_MICS           0x00000001       /* 10 micro second timestamp */
#define BL_OBJ_FLAG_TIME_ONE_NANS           0x00000002       /* 1 nano second timestamp */

#define BL_OBJ_SIGNATURE                    0x4A424F4C       /* object signature */
#define BL_FILE_SIGNATURE                   0x47474F4C       /* object signature */

BLFSTART
class TWBLF_API ObjectHeaderBase
{
public:
    uint32_t signature{BL_OBJ_SIGNATURE};
    uint16_t header_size{};
    uint16_t header_version{};
    uint32_t object_size{};
    uint32_t object_type{0};

    [[nodiscard]] uint16_t object_header_base_header_size() const
    {
        return sizeof(signature) + sizeof(header_size) + sizeof(header_version) + sizeof(object_size) + sizeof(object_type);
    }

    [[nodiscard]] uint32_t object_header_base_object_size() const
    {
        return sizeof(signature) + sizeof(header_size) + sizeof(header_version) + sizeof(object_size) + sizeof(object_type);
    }
};
BLFEND
#endif //OBJECTHEADERBASE_H
