#include "can_message_blf_writer.h"

#include "can_message.h"

namespace BLF
{

static WriterRegistrar<CanMessageBlfWriter> registrar(FileFormat::BLF, BusType::CAN);

bool CanMessageBlfWriter::write(const BusMessage& msg, FileWriter& writer) const
{
	const auto& can_msg = dynamic_cast<const CanMessage&>(msg);
	const CanFrame& can_frame = can_msg.get_frame();
	writer.append(reinterpret_cast<const uint8_t*>(&can_frame), sizeof(CanFrame));
	return true;
}
}
