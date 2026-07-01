#include "ethernet/ethernet_message.h"
#include "blf_frame_codec.h"
#include "blf_frame_registration.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>

namespace GWLogger::Blf
{

namespace
{

#pragma pack(push, 1)
struct EthernetFrameDiskHeader
{
	uint8_t source_address[6];
	uint16_t channel;
	uint8_t destination_address[6];
	uint16_t dir;
	uint16_t type;
	uint16_t tpid;
	uint16_t tci;
	uint16_t payload_length;
};
#pragma pack(pop)

class EthernetFrameBlfWriter final : public IMessageWriter
{
public:
	EthernetFrameBlfWriter()
	{
		header_base_.signature = BL_OBJ_SIGNATURE;
		header_base_.header_size = sizeof(ObjectHeaderBase) + sizeof(ObjectHeader);
		header_base_.header_version = 1;
		header_base_.object_type = BL_OBJ_TYPE_ETHERNET_FRAME;

		header_.client_index = 1;
		header_.object_version = 0;
		header_.time_flags = BL_OBJ_FLAG_TIME_ONE_NANS;
	}

	bool write(const BusMessage& msg, FileWriter& writer) override
	{
		const auto* ethernet_msg = dynamic_cast<const EthernetMessage*>(&msg);
		if (!ethernet_msg)
			return false;

		const auto& frame = ethernet_msg->get_frame();
		const auto payload_length = std::min<uint16_t>(
			frame.payload_length,
			ETHERNET_MAX_PAYLOAD_LENGTH);

		EthernetFrameDiskHeader frame_header{};
		std::copy_n(frame.source_address, 6, frame_header.source_address);
		frame_header.channel = frame.channel;
		std::copy_n(frame.destination_address, 6, frame_header.destination_address);
		frame_header.dir = frame.dir;
		frame_header.type = frame.type;
		frame_header.tpid = frame.tpid;
		frame_header.tci = frame.tci;
		frame_header.payload_length = payload_length;

		header_base_.object_size = static_cast<uint32_t>(
			sizeof(ObjectHeaderBase) + sizeof(ObjectHeader) +
			sizeof(EthernetFrameDiskHeader) + payload_length);
		header_.object_timestamp = encode_blf_timestamp(
			ethernet_msg->get_timestamp(),
			writer.get_file_start_time(),
			header_.time_flags);

		append_pod(writer, header_base_);
		append_pod(writer, header_);
		append_pod(writer, frame_header);
		writer.append(frame.payload, payload_length);
		return true;
	}

	[[nodiscard]] size_t estimate_write_size(const BusMessage& msg) const override
	{
		const auto* ethernet_msg = dynamic_cast<const EthernetMessage*>(&msg);
		if (!ethernet_msg)
			return 400;

		return sizeof(ObjectHeaderBase) + sizeof(ObjectHeader) +
			sizeof(EthernetFrameDiskHeader) +
			std::min<size_t>(
				ethernet_msg->get_frame().payload_length,
				ETHERNET_MAX_PAYLOAD_LENGTH);
	}

	void set_timestamp_unit(TimeStampUnit unit) override
	{
		switch (unit)
		{
		case TimeStampUnit::BLF_TIME_TEN_MICS:
			header_.time_flags = BL_OBJ_FLAG_TIME_TEN_MICS;
			break;
		case TimeStampUnit::BLF_TIME_ONE_NANS:
		default:
			header_.time_flags = BL_OBJ_FLAG_TIME_ONE_NANS;
			break;
		}
	}

private:
	ObjectHeaderBase header_base_{};
	ObjectHeader header_{};
};

} // namespace

GWLOGGER_REGISTER_BLF_WRITER(EthernetFrameBlfWriter, BusType::ETHERNET)

} // namespace GWLogger::Blf
