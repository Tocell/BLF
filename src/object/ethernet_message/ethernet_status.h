#ifndef ETHERNET_STATUS_H
#define ETHERNET_STATUS_H

#include "blf.h"
#include "object_header.h"

BLFSTART
    class TWBLF_API EthernetStatus : public ObjectHeader
{
public:
    uint16_t channel;
    uint16_t flags;                      /* Valid fields: */
                                               /* Bit 0 - Link Status */
                                               /* Bit 1 - Bitrate */
                                               /* Bit 2 - Ethernet Phy */
                                               /* Bit 3 - Duplex */
                                               /* Bit 4 - MDI Type */
                                               /* Bit 5 - Connector */
                                               /* Bit 6 - Clock Mode */
                                               /* Bit 7 - BR Pair */
                                               /* Bit 8 - mHardwareChannel */
                                               /* Bit 9 - mLinkUpDuration */
    uint8_t  link_status;                 /* Link Status:   */
                                               /* 0 - Unknown    */
                                               /* 1 - Link down  */
                                               /* 2 - Link up    */
                                               /* 3 - Negotiate  */
                                               /* 4 - Link error */
    uint8_t  ethernet_phy;                /* Ethernet Phy:     */
                                               /* 0 - Unknown      */
                                               /* 1 - IEEE 802.3   */
                                               /* 2 - BroadR-Reach */
    uint8_t  duplex;                     /* Duplex: */
                                               /* 0 - Unknown     */
                                               /* 1 - Half Duplex */
                                               /* 2 - Full Duplex */
    uint8_t  mdi;                        /* 0 - Unknown */
                                               /* 1 - Direct*/
                                               /* 2 - Crossover */
    uint8_t  connector;                  /* 0 - Unknown */
                                               /* 1 - RJ45*/
                                               /* 2 - D-Sub */
    uint8_t  clock_mode;                  /* 0 - Unknown */
                                               /* 1 - Master */
                                               /* 2 - Slave */
    uint8_t  pairs;                      /* 0 - Unknown */
                                               /* 1 - BR 1-pair*/
                                               /* 2 - BR 2-pair */
                                               /* 3 - BR 4-pair */
    uint8_t  hardware_channel;
    uint32_t bitrate;                    /* Bitrate in [kbit/sec] */

    // Version 1
    uint64_t link_up_duration;             /* Link up duration in ns */
};
BLFEND

#endif //ETHERNET_STATUS_H
