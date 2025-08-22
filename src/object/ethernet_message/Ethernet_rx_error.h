#ifndef ETHERNET_RX_ERROR_H
#define ETHERNET_RX_ERROR_H

#include "blf.h"
#include "object_header.h"
#include <vector>

// dir
#define ETHERNET_RX_ERROR_DIR_RX        0x00
#define ETHERNET_RX_ERROR_DIR_TX        0x01
#define ETHERNET_RX_ERROR_DIR_TXRQ      0x02

BLFSTART
    class TWBLF_API EthernetRxError : public ObjectHeader
{
public:
    uint16_t struct_length {};
    uint16_t channel {};
    uint16_t dir {};
    uint16_t hardware_channel {};
    uint32_t fcs {};
    uint16_t frame_data_length {};
    uint16_t reserved {};
    uint32_t error {};
    std::vector<uint8_t> frame_data {};
};
BLFEND

#endif //ETHERNET_RX_ERROR_H
