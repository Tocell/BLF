#include "can_message2_blf_writer.h"
#include "can_message2.h"
#include "writer_registrar.h"

namespace GWLogger::Blf
{

static WriterRegistrar<CanMessage2BlfWriter> registrar(FileFormat::BLF, BusType::CAN2);

CanMessage2BlfWriter::CanMessage2BlfWriter()
{
	header_base_.signature = BL_OBJ_SIGNATURE;
	header_base_.header_size = sizeof(ObjectHeaderBase) + sizeof(ObjectHeader);
	header_base_.object_size = sizeof(ObjectHeaderBase) + sizeof(ObjectHeader) + sizeof(CanFrame2);
	header_base_.object_type = BL_OBJ_TYPE_CAN_MESSAGE2;
	header_base_.header_version = 1;

	header_.client_index = 1;
	header_.object_timestamp = 0;
	header_.object_version = 0;
	header_.time_flags = BL_OBJ_FLAG_TIME_ONE_NANS;
}

void CanMessage2BlfWriter::set_timestamp_unit(int32_t unit)
{
	header_.time_flags = unit;
}

bool CanMessage2BlfWriter::write(const BusMessage& msg, FileWriter& writer)
{
	const auto& can_msg = dynamic_cast<const CanMessage2&>(msg);

	const CanFrame2& can_frame = can_msg.get_frame();

	if (BL_OBJ_FLAG_TIME_ONE_NANS == header_.time_flags)
	{
		header_.object_timestamp = can_msg.get_timestamp() - writer.get_file_start_time() * 1000ULL;
	}
	else if (BL_OBJ_FLAG_TIME_TEN_MICS == header_.time_flags)
	{
		uint64_t delta_ns = can_msg.get_timestamp() - writer.get_file_start_time() * 1000ULL;
		header_.object_timestamp = delta_ns / 10000ULL; // 10us = 10000ns
	}

	writer.append(reinterpret_cast<const uint8_t*>(&header_base_), sizeof(ObjectHeaderBase));
	writer.append(reinterpret_cast<const uint8_t*>(&header_), sizeof(ObjectHeader));
	writer.append(reinterpret_cast<const uint8_t*>(&can_frame), sizeof(CanFrame2));

	return true;
}

}

