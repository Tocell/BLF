#include "blf_frame_registration.h"
#include "can/can_driver_err_ext_message.h"

namespace GWLogger::Blf
{

GWLOGGER_REGISTER_BLF_FRAME_WRITER(CanDriverErrExtMessageBlfWriter, CanDriverErrExtMessage, CanDriverErrExtFrame, BL_OBJ_TYPE_CAN_DRIVER_ERROR_EXT, BusType::CAN_DRIVER_ERR_EXT)

}