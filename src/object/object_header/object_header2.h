#ifndef OBJECTHEADER2_H
#define OBJECTHEADER2_H

#include "object_header_base.h"

BLFSTART
class TWBLF_API ObjectHeader2 : public ObjectHeaderBase
{
public:
    uint32_t object_flags {BL_OBJ_FLAG_TIME_ONE_NANS};
    enum TimeStampStatus : uint8_t {
        Orig = 0x01,
        SwHw = 0x02,
        User = 0x10
    };
    uint8_t time_stamp_status {};
    uint8_t reserved_object_header{0};
    uint16_t object_version {0};
    uint64_t object_timestamp {0};
    uint64_t original_timestamp {0};

    [[nodiscard]] uint32_t object_header2_header_size() const
    {
        return object_header_base_header_size() + sizeof(object_flags) + sizeof(time_stamp_status) + sizeof(reserved_object_header) + sizeof(object_version) + sizeof(object_timestamp) + sizeof(original_timestamp);
    }

    [[nodiscard]] uint32_t object_header2_object_size() const
    {
        return object_header_base_object_size() + sizeof(object_flags) + sizeof(time_stamp_status) + sizeof(reserved_object_header) + sizeof(object_version) + sizeof(object_timestamp) + sizeof(original_timestamp);
    }
};
BLFEND

#endif //OBJECTHEADER2_H
