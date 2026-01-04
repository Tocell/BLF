#include "can_message_blf_writer.h"

namespace BLF
{

static WriterRegistrar<CanMessageBlfWriter> registrar(FileFormat::BLF, BusType::CAN);

bool CanMessageBlfWriter::write(const BusMessage& msg, FileWriter& writer) const
{
	writer.write_struct(msg);
	return true;
}
}

