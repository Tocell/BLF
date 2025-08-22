#ifndef CALCULATE_OBJECT_SIZE_H
#define CALCULATE_OBJECT_SIZE_H

#define BL_OBJ_TYPE_UNKNOWN                       0       /* unknown object */
#define BL_OBJ_TYPE_CAN_MESSAGE                   1       /* CAN message object */
#define BL_OBJ_TYPE_CAN_ERROR                     2       /* CAN error frame object */
#define BL_OBJ_TYPE_CAN_OVERLOAD                  3       /* CAN overload frame object */
#define BL_OBJ_TYPE_CAN_STATISTIC                 4       /* CAN driver statistics object */
#define BL_OBJ_TYPE_APP_TRIGGER                   5       /* application trigger object */
#define BL_OBJ_TYPE_ENV_INTEGER                   6       /* environment integer object */
#define BL_OBJ_TYPE_ENV_DOUBLE                    7       /* environment double object */
#define BL_OBJ_TYPE_ENV_STRING                    8       /* environment string object */
#define BL_OBJ_TYPE_ENV_DATA                      9       /* environment data object */
#define BL_OBJ_TYPE_LOG_CONTAINER                10       /* container object */

#define BL_OBJ_TYPE_LIN_MESSAGE                  11       /* LIN message object */
#define BL_OBJ_TYPE_LIN_CRC_ERROR                12       /* LIN CRC error object */
#define BL_OBJ_TYPE_LIN_DLC_INFO                 13       /* LIN DLC info object */
#define BL_OBJ_TYPE_LIN_RCV_ERROR                14       /* LIN receive error object */
#define BL_OBJ_TYPE_LIN_SND_ERROR                15       /* LIN send error object */
#define BL_OBJ_TYPE_LIN_SLV_TIMEOUT              16       /* LIN slave timeout object */
#define BL_OBJ_TYPE_LIN_SCHED_MODCH              17       /* LIN scheduler mode change object */
#define BL_OBJ_TYPE_LIN_SYN_ERROR                18       /* LIN sync error object */
#define BL_OBJ_TYPE_LIN_BAUDRATE                 19       /* LIN baudrate event object */
#define BL_OBJ_TYPE_LIN_SLEEP                    20       /* LIN sleep mode event object */
#define BL_OBJ_TYPE_LIN_WAKEUP                   21       /* LIN wakeup event object */

#define BL_OBJ_TYPE_MOST_SPY                     22       /* MOST spy message object */
#define BL_OBJ_TYPE_MOST_CTRL                    23       /* MOST control message object */
#define BL_OBJ_TYPE_MOST_LIGHTLOCK               24       /* MOST light lock object */
#define BL_OBJ_TYPE_MOST_STATISTIC               25       /* MOST statistic object */

#define BL_OBJ_TYPE_reserved_1                   26
#define BL_OBJ_TYPE_reserved_2                   27
#define BL_OBJ_TYPE_reserved_3                   28

#define BL_OBJ_TYPE_FLEXRAY_DATA                 29       /* FLEXRAY data object */
#define BL_OBJ_TYPE_FLEXRAY_SYNC                 30       /* FLEXRAY sync object */

#define BL_OBJ_TYPE_CAN_DRIVER_ERROR             31       /* CAN driver error object */

#define BL_OBJ_TYPE_MOST_PKT                     32       /* MOST Packet */
#define BL_OBJ_TYPE_MOST_PKT2                    33       /* MOST Packet including original timestamp */
#define BL_OBJ_TYPE_MOST_HWMODE                  34       /* MOST hardware mode event */
#define BL_OBJ_TYPE_MOST_REG                     35       /* MOST register data (various chips)*/
#define BL_OBJ_TYPE_MOST_GENREG                  36       /* MOST register data (MOST register) */
#define BL_OBJ_TYPE_MOST_NETSTATE                37       /* MOST NetState event */
#define BL_OBJ_TYPE_MOST_DATALOST                38       /* MOST data lost */
#define BL_OBJ_TYPE_MOST_TRIGGER                 39       /* MOST trigger */

#define BL_OBJ_TYPE_FLEXRAY_CYCLE                40       /* FLEXRAY V6 start cycle object */
#define BL_OBJ_TYPE_FLEXRAY_MESSAGE              41       /* FLEXRAY V6 message object */

#define BL_OBJ_TYPE_LIN_CHECKSUM_INFO            42       /* LIN checksum info event object */
#define BL_OBJ_TYPE_LIN_SPIKE_EVENT              43       /* LIN spike event object */

#define BL_OBJ_TYPE_CAN_DRIVER_SYNC              44       /* CAN driver hardware sync */

#define BL_OBJ_TYPE_FLEXRAY_STATUS               45       /* FLEXRAY status event object */

#define BL_OBJ_TYPE_GPS_EVENT                    46       /* GPS event object */

#define BL_OBJ_TYPE_FR_ERROR                     47       /* FLEXRAY error event object */
#define BL_OBJ_TYPE_FR_STATUS                    48       /* FLEXRAY status event object */
#define BL_OBJ_TYPE_FR_STARTCYCLE                49       /* FLEXRAY start cycle event object */
#define BL_OBJ_TYPE_FR_RCVMESSAGE                50       /* FLEXRAY receive message event object */

#define BL_OBJ_TYPE_REALTIMECLOCK                51       /* Realtime clock object */
#define BL_OBJ_TYPE_AVAILABLE2                   52       /* this object ID is available for the future */
#define BL_OBJ_TYPE_AVAILABLE3                   53       /* this object ID is available for the future */

#define BL_OBJ_TYPE_LIN_STATISTIC                54       /* LIN statistic event object */

#define BL_OBJ_TYPE_J1708_MESSAGE                55       /* J1708 message object */
#define BL_OBJ_TYPE_J1708_VIRTUAL_MSG            56       /* J1708 message object with more than 21 data bytes */

#define BL_OBJ_TYPE_LIN_MESSAGE2                 57       /* LIN frame object - extended */
#define BL_OBJ_TYPE_LIN_SND_ERROR2               58       /* LIN transmission error object - extended */
#define BL_OBJ_TYPE_LIN_SYN_ERROR2               59       /* LIN sync error object - extended */
#define BL_OBJ_TYPE_LIN_CRC_ERROR2               60       /* LIN checksum error object - extended */
#define BL_OBJ_TYPE_LIN_RCV_ERROR2               61       /* LIN receive error object */
#define BL_OBJ_TYPE_LIN_WAKEUP2                  62       /* LIN wakeup event object  - extended */
#define BL_OBJ_TYPE_LIN_SPIKE_EVENT2             63       /* LIN spike event object - extended */
#define BL_OBJ_TYPE_LIN_LONG_DOM_SIG             64       /* LIN long dominant signal object */

#define BL_OBJ_TYPE_APP_TEXT                     65       /* text object */

#define BL_OBJ_TYPE_FR_RCVMESSAGE_EX             66       /* FLEXRAY receive message ex event object */

#define BL_OBJ_TYPE_MOST_STATISTICEX             67       /* MOST extended statistic event */
#define BL_OBJ_TYPE_MOST_TXLIGHT                 68       /* MOST TxLight event */
#define BL_OBJ_TYPE_MOST_ALLOCTAB                69       /* MOST Allocation table event */
#define BL_OBJ_TYPE_MOST_STRESS                  70       /* MOST Stress event */

#define BL_OBJ_TYPE_ETHERNET_FRAME               71       /* Ethernet frame object */

#define BL_OBJ_TYPE_SYS_VARIABLE                 72       /* system variable object */

#define BL_OBJ_TYPE_CAN_ERROR_EXT                73       /* CAN error frame object (extended) */
#define BL_OBJ_TYPE_CAN_DRIVER_ERROR_EXT         74       /* CAN driver error object (extended) */

#define BL_OBJ_TYPE_LIN_LONG_DOM_SIG2            75       /* LIN long dominant signal object - extended */

#define BL_OBJ_TYPE_MOST_150_MESSAGE             76   /* MOST150 Control channel message */
#define BL_OBJ_TYPE_MOST_150_PKT                 77   /* MOST150 Asynchronous channel message */
#define BL_OBJ_TYPE_MOST_ETHERNET_PKT            78   /* MOST Ethernet channel message */
#define BL_OBJ_TYPE_MOST_150_MESSAGE_FRAGMENT    79   /* Partial transmitted MOST50/150 Control channel message */
#define BL_OBJ_TYPE_MOST_150_PKT_FRAGMENT        80   /* Partial transmitted MOST50/150 data packet on asynchronous channel */
#define BL_OBJ_TYPE_MOST_ETHERNET_PKT_FRAGMENT   81   /* Partial transmitted MOST Ethernet packet on asynchronous channel */
#define BL_OBJ_TYPE_MOST_SYSTEM_EVENT            82   /* Event for various system states on MOST */
#define BL_OBJ_TYPE_MOST_150_ALLOCTAB            83   /* MOST50/150 Allocation table event */
#define BL_OBJ_TYPE_MOST_50_MESSAGE              84   /* MOST50 Control channel message */
#define BL_OBJ_TYPE_MOST_50_PKT                  85   /* MOST50 Asynchronous channel message */

#define BL_OBJ_TYPE_CAN_MESSAGE2                 86   /* CAN message object - extended */

#define BL_OBJ_TYPE_LIN_UNEXPECTED_WAKEUP        87
#define BL_OBJ_TYPE_LIN_SHORT_OR_SLOW_RESPONSE   88
#define BL_OBJ_TYPE_LIN_DISTURBANCE_EVENT        89

#define BL_OBJ_TYPE_SERIAL_EVENT                 90

#define BL_OBJ_TYPE_OVERRUN_ERROR                91   /* driver overrun event */

#define BL_OBJ_TYPE_EVENT_COMMENT                92

#define BL_OBJ_TYPE_WLAN_FRAME                   93
#define BL_OBJ_TYPE_WLAN_STATISTIC               94

#define BL_OBJ_TYPE_MOST_ECL                     95   /* MOST Electrical Control Line event */

#define BL_OBJ_TYPE_GLOBAL_MARKER                96

#define BL_OBJ_TYPE_AFDX_FRAME                   97
#define BL_OBJ_TYPE_AFDX_STATISTIC               98

#define BL_OBJ_TYPE_KLINE_STATUSEVENT            99   /* E.g. wake-up pattern */

#define BL_OBJ_TYPE_CAN_FD_MESSAGE              100   /*CAN FD message object*/

#define BL_OBJ_TYPE_CAN_FD_MESSAGE_64           101   /*CAN FD message object */

#define BL_OBJ_TYPE_ETHERNET_RX_ERROR           102   /* Ethernet RX error object */
#define BL_OBJ_TYPE_ETHERNET_STATUS             103   /* Ethernet status object */

#define BL_OBJ_TYPE_CAN_FD_ERROR_64             104   /*CAN FD Error Frame object */
#define BL_OBJ_TYPE_LIN_SHORT_OR_SLOW_RESPONSE2 105

#define BL_OBJ_TYPE_AFDX_STATUS                 106   /* AFDX status object */
#define BL_OBJ_TYPE_AFDX_BUS_STATISTIC          107   /* AFDX line-dependent busstatistic object */
#define BL_OBJ_TYPE_reserved_4                  108
#define BL_OBJ_TYPE_AFDX_ERROR_EVENT            109   /* AFDX asynchronous error event*/

#define BL_OBJ_TYPE_A429_ERROR                  110   /* A429 error object */
#define BL_OBJ_TYPE_A429_STATUS                 111   /* A429 status object */
#define BL_OBJ_TYPE_A429_BUS_STATISTIC          112   /* A429 busstatistic object */
#define BL_OBJ_TYPE_A429_MESSAGE                113   /* A429 Message*/

#define BL_OBJ_TYPE_ETHERNET_STATISTIC          114   /* Ethernet statistic object */

#define BL_OBJ_TYPE_reserved_5                  115

#define BL_OBJ_TYPE_reserved_6                  116

#define BL_OBJ_TYPE_reserved_7                  117

#define BL_OBJ_TYPE_TEST_STRUCTURE              118   /* Event for test execution flow */

#define BL_OBJ_TYPE_DIAG_REQUEST_INTERPRETATION 119   /* Event for correct interpretation of diagnostic requests */

#define BL_OBJ_TYPE_ETHERNET_FRAME_EX           120   /* Ethernet packet extended object */
#define BL_OBJ_TYPE_ETHERNET_FRAME_FORWARDED    121   /* Ethernet packet forwarded object */
#define BL_OBJ_TYPE_ETHERNET_ERROR_EX           122   /* Ethernet error extended object */
#define BL_OBJ_TYPE_ETHERNET_ERROR_FORWARDED    123   /* Ethernet error forwarded object */

#define BL_OBJ_TYPE_FUNCTION_BUS                124   /* OBSOLETE */
#define BL_OBJ_TYPE_COMMUNICATION_OBJECT        124   /* Communication object in the communication setup */

#define BL_OBJ_TYPE_DATA_LOST_BEGIN             125   /* Data lost begin*/
#define BL_OBJ_TYPE_DATA_LOST_END               126   /* Data lost end*/
#define BL_OBJ_TYPE_WATER_MARK_EVENT            127   /* Watermark event*/
#define BL_OBJ_TYPE_TRIGGER_CONDITION           128   /* Trigger Condition event*/
#define BL_OBJ_TYPE_CAN_SETTING_CHANGED         129   /* CAN Settings Changed object */

#define BL_OBJ_TYPE_DISTRIBUTED_OBJECT_MEMBER   130   /* Distributed object member (communication setup)*/
#define BL_OBJ_TYPE_ATTRIBUTE_EVENT             131   /* ATTRIBUTE event (communication setup)*/
#define BL_OBJ_TYPE_DISTRIBUTED_OBJECT_CHANGE   132   /* Dynamic distributed object change (communication setup)*/
#define BL_OBJ_TYPE_ETHERNET_PHY_STATE          133   /* Ethernet Phy state object */

#define BL_OBJ_TYPE_MACSEC_STATUS               134   /* Macsec status object */
#define BL_OBJ_TYPE_reserved_8                  135
#define BL_OBJ_TYPE_10BASET1S_STATUS            136   /* 10BaseT1S status object */
#define BL_OBJ_TYPE_10BASET1S_STATISTIC         137   /* 10BaseT1S statistic object */

#define BL_DLC_TO_FRAME_LENGTH(dlc) \
    ((dlc) <= 8 ? (dlc) : \
    (dlc) == 9 ? 12 : \
    (dlc) == 10 ? 16 : \
    (dlc) == 11 ? 20 : \
    (dlc) == 12 ? 24 : \
    (dlc) == 13 ? 32 : \
    (dlc) == 14 ? 48 : \
    (dlc) == 15 ? 64 : -1)

#endif //CALCULATE_OBJECT_SIZE_H
