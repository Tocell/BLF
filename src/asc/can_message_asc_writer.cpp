#include "can_message_asc_writer.h"

namespace BLF
{

static WriterRegistrar<CanMessageAscWriter> registrar(FileFormat::ASC, BusType::CAN);

bool CanMessageAscWriter::write(const BusMessage& msg, FileWriter& writer)
{
	// const auto& can_msg = static_cast<const CanMessage&>(msg);
	writer.write_struct(msg);
	return true;
}

}
