#ifndef CAN_SETTING_CHANGED_H
#define CAN_SETTING_CHANGED_H

#include "../common.h"
#include "object_header.h"
#include "canfd_ext_frame_data.h"

BLFSTART
class TWBLF_API CanSettingChanged final : public ObjectHeader
{
public:
    uint16_t channel {};
    uint8_t changed_type {};
    CanFdExtFrameData bitTimings;

    [[nodiscard]] uint32_t can_setting_changed_object_size() const
    {
        return object_header_object_size() +
               sizeof(channel) +
               sizeof(changed_type) +
               bitTimings.canfd_ext_frame_data_object_size();
    }
};
BLFEND

#endif //CAN_SETTING_CHANGED_H
