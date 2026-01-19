#include "../include/message_factory.h"
#include "../include/bus_message.h"

namespace BLF
{

BLF_API void destroy_message(BusMessage* message)
{
	delete message;
}

}