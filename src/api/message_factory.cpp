#include "../include/message_factory.h"
#include "../include/types.h"

namespace BLF
{

BLF_API void destroy_message(BusMessage* message)
{
	delete message;
}

}