#include "blf_frame_registration.h"
#include "can/can_error_message.h"

namespace GWLogger::Blf
{

GWLOGGER_REGISTER_BLF_FRAME_WRITER(CanErrMessageBlfWriter, CanErrMessage, CanErrFrame, BL_OBJ_TYPE_CAN_ERROR, BusType::CAN_ERR)

}