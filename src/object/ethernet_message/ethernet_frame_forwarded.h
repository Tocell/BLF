#ifndef ETHERNET_FRAME_FORWARDED_H
#define ETHERNET_FRAME_FORWARDED_H

#include "blf.h"
#include "object_header.h"
#include <vector>

BLFSTART
    class TWBLF_API EthernetFrameForwarded : public ObjectHeader
{
public:
    uint16_t structLength {};
    uint16_t flags {};
    uint16_t channel {};
    uint16_t hardwareChannel {};
    uint64_t frameDuration {};
    uint32_t frameChecksum {};
    uint16_t dir {};
    uint16_t frameLength {};
    uint32_t frameHandle {};
    uint32_t reservedEthernetFrameForwarded {};
    std::vector<uint8_t> frameData {};
};
BLFEND

#endif //ETHERNET_FRAME_FORWARDED_H
