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

struct CanFrame2
{
	uint16_t channel;
	uint8_t flags;
	uint8_t dlc;
	uint32_t id;
	uint8_t data[8];
	uint32_t frame_length; // 帧持续时间 (ns)，不填可能导致统计里的 BusLoad 为 0
	uint8_t bit_count;     // 总位宽
	uint8_t reserved1;
	uint16_t reserved2;
};

struct CanFdExtFrameData
{
    uint32_t btr_ext_arb;
    uint32_t btr_ext_data;
};

enum Flags : uint8_t
{
	TX = (1 << 0),
	NERR = (1 << 5),
	WU = (1 << 6),
	RTR = (1 << 7)
};

enum CanFdFlags : uint8_t
{
	EDL = (1 << 0),
	BRS = (1 << 1),
	ESI = (1 << 2)
};

struct CanFdFrame
{
	uint16_t channel;
	uint8_t flags;
	uint8_t dlc;
	uint32_t id;
	uint32_t frame_length;
	uint8_t arb_bit_count;
	uint8_t can_fd_flags;
	uint8_t valid_data_bytes;
	uint8_t reserved1;
	uint32_t reserved2;
	uint8_t data[64];
	uint32_t reserved3;
};

struct CanFDFrame64
{
    uint8_t channel;
    uint8_t dlc;
    uint8_t valid_data_bytes;
    uint8_t tx_count;
    uint32_t id;
    uint32_t frame_length;
    uint32_t flags;
    uint32_t btr_cfg_arb;
    uint32_t btr_cfg_data;
    uint32_t time_offset_brs_ns;
    uint32_t time_offset_crc_del_ns;
    uint16_t bit_count;
    uint8_t dir;
    uint8_t ext_data_offset;
    uint32_t crc;
	uint8_t data[64];
};

struct CanDriverError
{
	uint16_t channel;
	uint8_t tx_errors;
	uint8_t rx_errors;
	uint32_t error_code;
};

struct CanDriverErrorExt
{
	uint16_t channel;
	uint8_t tx_errors;
	uint8_t rx_errors;
	uint32_t error_code;
	uint32_t flags;
	uint8_t state;
	uint8_t reserved1;
	uint16_t reserved2;
	uint32_t reserved3[4];
};

struct CanDriverHwSync
{
	uint16_t channel;
	uint8_t flags;
	uint8_t reserved1;
	uint32_t reserved2;
};

struct CanDriverStatistic
{
	uint16_t channel;
	uint16_t busLoad;
	uint32_t standard_data_frames;
	uint32_t extended_data_frames;
	uint32_t standard_remote_frames;
	uint32_t extended_remote_frames;
	uint32_t error_frames;
	uint32_t overloadFrames;
	uint32_t reserved_can_driver_statistic;
};

struct CanErrorFrame
{
	uint16_t channel;
	uint16_t length;
	uint32_t reserved;
};

struct CanErrorFrameExt
{
	uint16_t channel;
	uint16_t length;
	uint32_t flags;
	uint8_t ecc;
	uint8_t position;
	uint8_t dlc;
	uint8_t reserved1;
	uint32_t frame_length_in_ns;
	uint32_t id;
	uint16_t flags_ext;
	uint16_t reserved2;
	uint8_t data[8];
};

struct CanFdErrorFrame64
{
	uint8_t channel;
	uint8_t dlc;
	uint8_t valid_data_bytes;
	uint8_t ecc;
	uint16_t flags;
	uint16_t error_code_ext;
	uint16_t extflags;
	uint8_t ext_data_offset;
	uint8_t reserved1;
	uint32_t id;
	uint32_t frame_length;
	uint32_t brt_cfg_arb;
	uint32_t brt_cfg_data;
	uint32_t time_offset_brs_ns;
	uint32_t time_offset_crc_del_ns;
	uint32_t crc;
	uint16_t error_position;
	uint16_t reserved2;
	uint8_t data[64];
	uint8_t reserved3[64];
};

struct CanOverloadFrame
{
	uint16_t channel;
	uint16_t reserved1;
	uint32_t reserved2;
};

struct CanSettingChanged
{
	uint16_t channel;
	uint8_t change_type;
	CanFdExtFrameData bit_timings;
};


#pragma pack(pop)


static_assert(sizeof(BLF::CanFrame2) == 24, "CanFrame2 must be 24 bytes (VBLCANMessage2)");
static_assert(sizeof(BLF::CanFrame)  == 16, "CanFrame must be 16 bytes (VBLCANMessage)");
static_assert(sizeof(CanFrame) == 16, "CanFrame must be 16 bytes");

}
#endif //OBJECT_HEADER_H
