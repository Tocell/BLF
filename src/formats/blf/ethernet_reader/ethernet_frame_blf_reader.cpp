#include "ethernet/ethernet_message.h"
#include "blf_frame_codec.h"
#include "blf_frame_registration.h"

#include <algorithm>
#include <cstddef>
#include <cstring>

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

class EthernetFrameBlfReader final : public IMessageReader
{
public:
	BusMessagePtr read(const uint8_t* obj_begin, size_t obj_size) override
	{
		constexpr size_t kFrameOffset = sizeof(ObjectHeaderBase) + sizeof(ObjectHeader);
		constexpr size_t kMinSize = kFrameOffset + sizeof(EthernetFrameDiskHeader);

		if (!obj_begin || obj_size < kMinSize)
			return nullptr;

		ObjectHeaderBase base{};
		std::memcpy(&base, obj_begin, sizeof(base));
		if (base.signature != BL_OBJ_SIGNATURE || base.object_type != BL_OBJ_TYPE_ETHERNET_FRAME)
			return nullptr;

		EthernetFrameDiskHeader disk{};
		std::memcpy(&disk, obj_begin + kFrameOffset, sizeof(disk));

		const size_t available_payload = obj_size - kMinSize;
		const size_t payload_length = std::min<size_t>(
			std::min<size_t>(disk.payload_length, ETHERNET_MAX_PAYLOAD_LENGTH),
			available_payload);

		EthernetFrame frame{};
		std::copy_n(disk.source_address, 6, frame.source_address);
		frame.channel = disk.channel;
		std::copy_n(disk.destination_address, 6, frame.destination_address);
		frame.dir = disk.dir;
		frame.type = disk.type;
		frame.tpid = disk.tpid;
		frame.tci = disk.tci;
		frame.payload_length = static_cast<uint16_t>(payload_length);
		std::memcpy(frame.payload, obj_begin + kMinSize, payload_length);

		return make_message(frame);
	}
};

} // namespace

GWLOGGER_REGISTER_BLF_READER(EthernetFrameBlfReader, BL_OBJ_TYPE_ETHERNET_FRAME)

} // namespace GWLogger::Blf
