#include "blf_frame_registration.h"
#include "flexray/flexray_message.h"

namespace GWLogger::Blf
{

GWLOGGER_REGISTER_BLF_FRAME_WRITER(FlexRayMessageBlfWriter, FlexRayMessage, FlexRayFrame, BL_OBJ_TYPE_FLEXRAY_MESSAGE, BusType::FLEXRAY)

}
