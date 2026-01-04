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
#pragma pack(pop)

}
#endif //OBJECT_HEADER_H
