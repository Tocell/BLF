#include "canfd_message_asc_writer.h"
#include "canfd_message.h"

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

namespace GWLogger::Asc
{

static WriterRegistrar<CanFdMessageAscWriter> registrar(FileFormat::ASC, BusType::CAN_FD);

CanFdMessageAscWriter::CanFdMessageAscWriter():
	timestamp_unit_(0)
{
}

void CanFdMessageAscWriter::set_timestamp_unit(TimeStampUnit unit)
{
	timestamp_unit_ = static_cast<uint32_t>(unit);
}

static inline const char* asc_dir_from_flags(uint8_t flags)
{
    if (flags == 1) return "Tx";
    if (flags == 2) return "Rx";
    return "Rx";
}

// Vector: extended id 以 'x' 结尾（示例 10001x），且仅 hex
static inline bool asc_is_extended_id(uint32_t id)
{
    return id > 0x7FFu;
}

// CAN FD DLC -> DataLength 映射（当 valid_data_bytes 没填时用）
static inline uint8_t canfd_dlc_to_len(uint8_t dlc)
{
    if (dlc <= 8) return dlc;
    switch (dlc)
    {
        case 9:  return 12;
        case 10: return 16;
        case 11: return 20;
        case 12: return 24;
        case 13: return 32;
        case 14: return 48;
        case 15: return 64;
        default: return 0;
    }
}

// <Time> CANFD <Channel> <Dir> <ID> <SymbolicName> <BRS> <ESI> <DLC>
// <DataLength> <D1> … <D8> <MessageDuration> <MessageLength> <Flags>
// <CRC> <BitTimingConfArb> <BitTimingConfData> <BitTimingConfExtArb>
// <BitTimingConfExtData>
static inline std::string build_canfd_asc_line(
    const CanFdFrame& f,
    uint64_t msg_timestamp_us,
    uint64_t file_start_time_us,
    bool timestamps_absolute)
{
    // <Time> 单位：秒（浮点）
    double t_sec = 0.0;
    if (timestamps_absolute) {
        t_sec = static_cast<double>(msg_timestamp_us) / 1e6;
    } else {
        const uint64_t base = std::min(msg_timestamp_us, file_start_time_us);
        t_sec = static_cast<double>(msg_timestamp_us - base) / 1e6;
    }

    // BRS/ESI：来自 can_fd_flags（你 enum 里 BRS=bit1, ESI=bit2）
    const int brs = (f.can_fd_flags & (1u << 1)) ? 1 : 0;
    const int esi = (f.can_fd_flags & (1u << 2)) ? 1 : 0;

    // DataLength：优先用 valid_data_bytes，否则用 DLC 映射
    uint8_t data_len = f.valid_data_bytes;
    if (data_len == 0) data_len = canfd_dlc_to_len(f.dlc);
    data_len = std::min<uint8_t>(data_len, 64);

    // MessageDuration：Vector 这里是 ns；你结构体里 frame_length 看起来就是 duration(ns)
    const uint32_t msg_duration_ns = f.frame_length;

    // MessageLength：你结构体没有 “总 bit 数”，先写 0
    const uint32_t msg_length_bits = 0;

    // Flags：按 Vector 的 bitfield 习惯，至少把 FDF/BRS/ESI 打上（其余没有就 0）
    // Vector Flags bit12=FDF, bit13=BRS, bit14=ESI（见文档 Flags 定义）
    uint32_t flags = 0;
    flags |= (1u << 12);           // FDF = 1 (CAN FD)
    if (brs) flags |= (1u << 13);
    if (esi) flags |= (1u << 14);

    // CRC / BitTiming：拿不到就写 0（Vector 文档允许不支持时为 0）
    const uint32_t crc = 0;
    const uint32_t btr_arb = 0, btr_data = 0, btr_ext_arb = 0, btr_ext_data = 0;

    std::ostringstream oss;
    oss.setf(std::ios::fixed);
    oss << std::setprecision(6);

    // <Time> CANFD <Channel> <Dir> <ID> <SymbolicName> <BRS> <ESI> <DLC> <DataLength> ...
    oss << t_sec << " CANFD "
        << f.channel << ' '
        << asc_dir_from_flags(f.flags) << ' ';

    // ID：hex，扩展帧加 'x'
    oss << std::hex << std::uppercase;
    if (asc_is_extended_id(f.id)) {
        oss << (f.id & 0x1FFFFFFFu) << 'x';
    } else {
        oss << (f.id & 0x7FFu);
    }

    // SymbolicName：你目前没做 symbolic logging，就按示例用 0 占位
    oss << std::dec << " 0 "
        << brs << ' '
        << esi << ' ';

    // DLC：Vector 规定是 hex 0..F（无 0x）
    oss << std::hex << std::uppercase
        << static_cast<int>(f.dlc)
        << std::dec << ' ';

    // DataLength：十进制
    oss << static_cast<int>(data_len);

    // <D1> … <D64>：CAN FD 数据字节只用 2 位 hex
    oss << std::hex << std::uppercase << std::setfill('0');
    for (int i = 0; i < static_cast<int>(data_len); ++i)
    {
        oss << ' ' << std::setw(2) << static_cast<int>(f.data[i]);
    }
    oss << std::dec;

    // <MessageDuration> <MessageLength>：十进制
    oss << ' ' << msg_duration_ns
        << ' ' << msg_length_bits;

    // <Flags> <CRC> <BitTiming...>：这些在示例里看起来是 hex，无 0x
    oss << ' ' << std::hex << std::uppercase
        << flags
        << ' ' << crc
        << ' ' << btr_arb
        << ' ' << btr_data
        << ' ' << btr_ext_arb
        << ' ' << btr_ext_data
        << "\r\n";

    return oss.str();
}

bool CanFdMessageAscWriter::write(const BusMessage& msg, FileWriter& writer)
{
    const auto& canfd_msg = dynamic_cast<const CanFdMessage&>(msg);
    const auto& f = canfd_msg.get_frame();

    auto line = build_canfd_asc_line(
        f,
        canfd_msg.get_timestamp(),
        writer.get_file_start_time(),
        false /* relative timestamps */);

    writer.append(reinterpret_cast<const uint8_t*>(line.data()), line.size());
    return true;
}

}