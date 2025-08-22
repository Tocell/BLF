#ifndef ETHERNET_FRAME_H
#define ETHERNET_FRAME_H

#include "blf.h"
#include "object_header.h"
#include <vector>
#include <array>

#define ETHERNET_FRAME_DIR_RX       0x00
#define ETHERNET_FRAME_DIR_TX       0x01
#define ETHERNET_FRAME_DIR_TXRQ     0x02

BLFSTART
    class TWBLF_API EthernetFrame : public ObjectHeader
{
public:
    std::array<uint8_t, 6> source_address {};
    uint16_t channel {};
    std::array<uint8_t, 6> destination_address {};
    uint16_t dir {};
    uint16_t type {};
    uint16_t tpid {};
    uint16_t tci {};
    uint16_t payLoad_length {};
    std::vector<uint8_t> payload {};
};
BLFEND

#endif //ETHERNET_FRAME_H
