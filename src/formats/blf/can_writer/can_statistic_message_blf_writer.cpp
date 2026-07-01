#include "blf_frame_registration.h"
#include "can/can_statistic_message.h"

namespace GWLogger::Blf
{

GWLOGGER_REGISTER_BLF_FRAME_WRITER(CanStatisticMessageBlfWriter, CanStatisticMessage, CanDriverStatisticFrame, BL_OBJ_TYPE_CAN_STATISTIC, BusType::CAN_STATISTIC)

}