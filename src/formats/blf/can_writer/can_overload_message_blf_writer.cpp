#include "blf_frame_registration.h"
#include "can/can_overload_message.h"

namespace GWLogger::Blf
{

GWLOGGER_REGISTER_BLF_FRAME_WRITER(CanOverloadMessageBlfWriter, CanOverloadMessage, CanOverloadFrame, BL_OBJ_TYPE_CAN_OVERLOAD, BusType::CAN_OVERLOAD)

}