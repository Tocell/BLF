#ifndef LIN_OBJECT_H
#define LIN_OBJECT_H

#include <cstdint>

namespace GWLogger
{

#pragma pack(push, 1)

struct LinFrame
{
	uint16_t channel;
	uint8_t id;
	uint8_t dlc;
	uint8_t data[8];
	uint8_t fsm_id;
	uint8_t fsm_state;
	uint8_t header_time;
	uint8_t full_time;
	uint16_t crc;
	uint8_t dir;
	uint8_t reserved;
};

#pragma pack(pop)

}

#endif //LIN_OBJECT_H
