#include "blf_frame_registration.h"
#include "can/can_err_ext_message.h"

namespace GWLogger::Blf
{

GWLOGGER_REGISTER_BLF_FRAME_WRITER(CanErrExtMessageBlfWriter, CanErrExtMessage, CanErrExtFrame, BL_OBJ_TYPE_CAN_ERROR_EXT, BusType::CAN_ERR_EXT)

}