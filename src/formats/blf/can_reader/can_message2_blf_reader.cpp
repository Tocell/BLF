#include "blf_frame_registration.h"
#include "can/can_message2.h"

namespace GWLogger::Blf
{

GWLOGGER_REGISTER_BLF_FRAME_READER(CanMessage2BlfReader, CanFrame2, BL_OBJ_TYPE_CAN_MESSAGE2)

}