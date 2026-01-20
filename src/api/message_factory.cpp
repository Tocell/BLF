#include "message_factory.h"
#include "bus_message.h"

namespace GWLogger
{

GWLOGGER_API void destroy_message(BusMessage* message)
{
	delete message;
}

}