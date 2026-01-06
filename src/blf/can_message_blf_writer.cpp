#include "can_message_blf_writer.h"

#include "can_message.h"

#include <iostream>

namespace BLF
{

static WriterRegistrar<CanMessageBlfWriter> registrar(FileFormat::BLF, BusType::CAN);

CanMessageBlfWriter::CanMessageBlfWriter()
{
	header_base_.signature = BL_OBJ_SIGNATURE;
	header_base_.header_size = sizeof(ObjectHeaderBase) + sizeof(ObjectHeader);
	header_base_.object_size = sizeof(ObjectHeaderBase) + sizeof(ObjectHeader) + sizeof(CanFrame);
	header_base_.object_type = BL_OBJ_TYPE_CAN_MESSAGE;
	header_base_.header_version = 1;

	header_.client_index = 1;
	header_.object_timestamp = 0;
	header_.object_version = 0;
	header_.time_flags = BL_OBJ_FLAG_TIME_ONE_NANS;
}


bool CanMessageBlfWriter::write(const BusMessage& msg, FileWriter& writer)
{
	const auto& can_msg = dynamic_cast<const CanMessage&>(msg);

	const CanFrame& can_frame = can_msg.get_frame();

	header_.object_timestamp = can_msg.get_timestamp() * 1000;
	header_.object_timestamp = can_msg.get_timestamp();
	writer.append(reinterpret_cast<const uint8_t*>(&header_base_), sizeof(ObjectHeaderBase));
	writer.append(reinterpret_cast<const uint8_t*>(&header_), sizeof(ObjectHeader));
	writer.append(reinterpret_cast<const uint8_t*>(&can_frame), sizeof(CanFrame));

	return true;
}
}
