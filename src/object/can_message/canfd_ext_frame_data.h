#ifndef CANFD_EXT_FRAME_DATA_H
#define CANFD_EXT_FRAME_DATA_H

#include "../common.h"
#include "cstdint"

BLFSTART
class TWBLF_API CanFdExtFrameData
{
public:
    uint32_t btr_ext_arb {};
    uint32_t btr_ext_data {};
    // std::vector<uint8_t> reserved_canfd_ext_frame_data {}; //may be extended in future versions
    [[nodiscard]] uint32_t canfd_ext_frame_data_object_size() const
    {
        return sizeof(btr_ext_arb) + sizeof(btr_ext_data);
    }
};
BLFEND

#endif //CANFD_EXT_FRAME_DATA_H
