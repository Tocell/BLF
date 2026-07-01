#include "blf_frame_registration.h"
#include "can/canfd_message.h"

namespace GWLogger::Blf
{

GWLOGGER_REGISTER_BLF_FRAME_WRITER(CanFdMessageBlfWriter, CanFdMessage, CanFdFrame, BL_OBJ_TYPE_CAN_FD_MESSAGE, BusType::CAN_FD)

}