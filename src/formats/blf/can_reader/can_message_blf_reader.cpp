#include "blf_frame_registration.h"
#include "can/can_message.h"

namespace GWLogger::Blf
{

GWLOGGER_REGISTER_BLF_FRAME_READER(CanMessageBlfReader, CanFrame, BL_OBJ_TYPE_CAN_MESSAGE)

}