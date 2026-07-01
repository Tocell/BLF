#include "blf_frame_registration.h"
#include "can/canfd_message64.h"

namespace GWLogger::Blf
{

GWLOGGER_REGISTER_BLF_FRAME_READER(CanFdMessage64BlfReader, CanFdFrame64, BL_OBJ_TYPE_CAN_FD_MESSAGE_64)

}