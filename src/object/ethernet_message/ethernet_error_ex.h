#ifndef ETHERNET_ERROR_EX_H
#define ETHERNET_ERROR_EX_H

#include "blf.h"
#include "object_header.h"
#include <vector>

BLFSTART
    class TWBLF_API EthernetErrorEx : public ObjectHeader
{
public:
    uint16_t struct_length {};
    uint16_t flags {};
    uint16_t channel {};
    uint16_t hardware_channel {};
    uint64_t frame_duration {};
    uint32_t frame_checksum {};
    uint16_t dir {};
    uint16_t frame_length {};
    uint32_t frame_handle {};
    uint32_t error {};
    std::vector<uint8_t> frame_data {};
};
BLFEND

#endif //ETHERNET_ERROR_EX_H
