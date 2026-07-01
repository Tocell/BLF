#include "blf_frame_registration.h"
#include "can/canfd_err64_message.h"

namespace GWLogger::Blf
{

GWLOGGER_REGISTER_BLF_FRAME_READER(CanErr64MessageBlfReader, CanFdErr64Frame, BL_OBJ_TYPE_CAN_FD_ERROR_64)

}