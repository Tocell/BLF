#ifndef ETHERNET_STATISTIC_H
#define ETHERNET_STATISTIC_H

#include "blf.h"
#include "object_header.h"
#include <vector>

BLFSTART
    class TWBLF_API EthernetStatistic : public ObjectHeader
{
public:
    uint16_t channel {};
    uint64_t rcv_ok_hw {};
    uint64_t xmit_ok_hw {};
    uint64_t rcv_error_hw {};
    uint64_t xmit_error_hw {};
    uint64_t rcv_bytes_hw {};
    uint64_t xmit_bytes_hw {};
    uint64_t rcv_no_buffer_hw {};
    int16_t sqi {};
    uint16_t hardware_channel {};
};
BLFEND

#endif //ETHERNET_STATISTIC_H
