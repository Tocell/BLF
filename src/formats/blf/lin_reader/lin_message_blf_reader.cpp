#include "blf_frame_registration.h"
#include "lin/lin_message.h"

namespace GWLogger::Blf
{

GWLOGGER_REGISTER_BLF_FRAME_READER(LinMessageBlfReader, LinFrame, BL_OBJ_TYPE_LIN_MESSAGE)

}
