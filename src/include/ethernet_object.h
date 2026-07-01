#ifndef ETHERNET_OBJECT_H
#define ETHERNET_OBJECT_H

#include <cstdint>

namespace GWLogger
{

constexpr uint16_t ETHERNET_MAX_PAYLOAD_LENGTH = 1582;

#pragma pack(push, 1)

struct EthernetFrame
{
	uint8_t source_address[6];
	uint16_t channel;
	uint8_t destination_address[6];
	uint16_t dir;
	uint16_t type;
	uint16_t tpid;
	uint16_t tci;
	uint16_t payload_length;
	uint8_t payload[ETHERNET_MAX_PAYLOAD_LENGTH];
};

#pragma pack(pop)

}

#endif //ETHERNET_OBJECT_H
