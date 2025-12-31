#include "can_message_blf_writer.h"

namespace BLF
{
bool CanMessageWriter::write(const BusMessage& msg, std::ostream& out_stream) const
{
	return true;
}
}

