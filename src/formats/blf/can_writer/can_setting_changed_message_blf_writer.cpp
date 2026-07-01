#include "blf_frame_registration.h"
#include "can/can_setting_change_message.h"

namespace GWLogger::Blf
{

GWLOGGER_REGISTER_BLF_FRAME_WRITER(CanSettingChangedMessageBlfWriter, CanSettingChangedMessage, CanSettingChangedFrame, BL_OBJ_TYPE_CAN_SETTING_CHANGED, BusType::CAN_SETTING_CHANGE)

}