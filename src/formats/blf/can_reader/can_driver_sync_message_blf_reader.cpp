#include "blf_frame_registration.h"
#include "can/can_driver_sync_message.h"

namespace GWLogger::Blf
{

GWLOGGER_REGISTER_BLF_FRAME_READER(CanDriverSyncMessageBlfReader, CanDriverSyncFrame, BL_OBJ_TYPE_CAN_DRIVER_SYNC)

}