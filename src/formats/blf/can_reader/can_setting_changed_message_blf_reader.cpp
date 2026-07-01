#include "blf_frame_registration.h"
#include "can/can_setting_change_message.h"

namespace GWLogger::Blf
{

GWLOGGER_REGISTER_BLF_FRAME_READER(CanSettingChangedMessageBlfReader, CanSettingChangedFrame, BL_OBJ_TYPE_CAN_SETTING_CHANGED)

}