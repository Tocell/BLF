#ifndef OBJECT_HEADER_H
#define OBJECT_HEADER_H

#include <cstdint>

namespace BLF
{
#pragma pack(push, 1)
struct CanFrame
{
	uint16_t channel;
	uint8_t flags;
	uint8_t dlc;
	uint32_t id;
	uint8_t data[8];
};

struct CanFrame2
{
	uint16_t channel;
	uint8_t flags;       // CAN_MSG_DIR, RTR, etc.
	uint8_t dlc;
	uint32_t id;
	uint8_t data[8];
	uint32_t frame_length; // 帧持续时间 (ns)，不填可能导致统计里的 BusLoad 为 0
	uint8_t bit_count;     // 总位宽
	uint8_t reserved1;
	uint16_t reserved2;
};
#pragma pack(pop)


static_assert(sizeof(BLF::CanFrame2) == 24, "CanFrame2 must be 24 bytes (VBLCANMessage2)");
static_assert(sizeof(BLF::CanFrame)  == 16, "CanFrame must be 16 bytes (VBLCANMessage)");
static_assert(sizeof(CanFrame) == 16, "CanFrame must be 16 bytes");

}
#endif //OBJECT_HEADER_H
