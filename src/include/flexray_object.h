#ifndef FLEXRAY_OBJECT_H
#define FLEXRAY_OBJECT_H

#include <cstdint>

namespace GWLogger
{

#pragma pack(push, 1)

struct FlexRayFrame
{
	uint16_t channel;
	uint8_t dir;
	uint8_t low_time;
	uint32_t fpga_tick;
	uint32_t fpga_tick_overflow;
	uint32_t client_index;
	uint32_t cluster_time;
	uint16_t frame_id;
	uint16_t header_crc;
	uint16_t frame_state;
	uint8_t length;
	uint8_t cycle;
	uint8_t header_bit_mask;
	uint8_t reserved1;
	uint16_t reserved2;
	uint8_t data[64];
};

#pragma pack(pop)

}

#endif //FLEXRAY_OBJECT_H
