#include "blf_frame_registration.h"
#include "lin/lin_message.h"

namespace GWLogger::Blf
{

GWLOGGER_REGISTER_BLF_FRAME_WRITER(LinMessageBlfWriter, LinMessage, LinFrame, BL_OBJ_TYPE_LIN_MESSAGE, BusType::LIN)

}
