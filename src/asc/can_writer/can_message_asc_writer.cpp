#include "can_message_asc_writer.h"
#include "can_message.h"

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

namespace GWLogger::Asc
{

static WriterRegistrar<CanMessageAscWriter> registrar(FileFormat::ASC, BusType::CAN);

CanMessageAscWriter::CanMessageAscWriter():
	timestamp_unit_(0)
{
}

void CanMessageAscWriter::set_timestamp_unit(TimeStampUnit unit)
{
	timestamp_unit_ = static_cast<uint32_t>(unit);
}


static inline const char* asc_dir_from_flags(uint8_t flags)
{
	if (flags == 1) return "Tx";
	if (flags == 2) return "Rx";
	return "Rx"; // fallback
}

// 若你们没有显式 IDE 标志，只能用 id>0x7FF 推断扩展帧
static inline bool asc_is_extended_id(uint32_t id)
{
	return id > 0x7FFu;
}

static inline std::string build_canframe_asc_line(
	const CanFrame& f,
	uint64_t msg_timestamp_us,      // 绝对时间戳(us)
	uint64_t file_start_time_us,    // 文件起始时间戳(us)，用于输出相对秒
	bool timestamps_absolute // false: 相对文件起始；true: 直接输出 epoch 秒（一般不建议）
)
{
	// ASC <Time> 单位：秒（浮点小数）
	double t_sec = 0.0;
	if (timestamps_absolute) {
		t_sec = static_cast<double>(msg_timestamp_us) / 1e6;
	} else {
		// 防止乱序导致负数
		const uint64_t base = std::min(msg_timestamp_us, file_start_time_us);
		t_sec = static_cast<double>(msg_timestamp_us - base) / 1e6;
	}

	std::ostringstream oss;
	oss.setf(std::ios::fixed);
	oss << std::setprecision(6); // 可改 4/9

	// <Time> <Channel> <ID> <Dir> d <DLC> <Data...>\r\n
	oss << t_sec << ' '
		<< f.channel << ' ';

	// ID：standard 3位hex；extended 8位hex（基础兼容写法）
	oss << std::hex << std::uppercase << std::setfill('0');
	if (asc_is_extended_id(f.id)) {
		oss << std::setw(8) << f.id;
	} else {
		oss << std::setw(3) << (f.id & 0x7FFu);
	}
	oss << std::dec;

	// Dir
	oss << ' ' << asc_dir_from_flags(f.flags);

	// Data frame：先按 d 输出（如果你后续支持 remote frame，可在这里改成 r）
	oss << " d " << static_cast<int>(f.dlc);

	// Data bytes：按 DLC 输出，最多 8
	const int dlc = std::clamp<int>(static_cast<int>(f.dlc), 0, 8);
	for (int i = 0; i < dlc; ++i)
	{
		oss << ' ' << std::hex << std::uppercase
			<< std::setw(2) << std::setfill('0')
			<< static_cast<int>(f.data[i]);
	}

	oss << "\r\n";
	return oss.str();
}

// v7.5
// <Time> <Channel> <ID> <Dir> d <DLC> <D0> <D1>...<D8> Length =
// <MessageDuration> BitCount = <MessageLength> <MessageFlags>
static inline std::string build_can_asc_line_full(
    const CanFrame& f,
    uint64_t msg_timestamp_us,      // 消息时间戳(us)
    uint64_t file_start_time_us,    // 文件起始时间戳(us)
    bool timestamps_absolute        // false: 相对；true: 绝对
)
{
    // <Time> 单位：秒
    double t_sec = 0.0;
    if (timestamps_absolute) {
        t_sec = static_cast<double>(msg_timestamp_us) / 1e6;
    } else {
        const uint64_t base = std::min(msg_timestamp_us, file_start_time_us);
        t_sec = static_cast<double>(msg_timestamp_us - base) / 1e6;
    }

    // Type：Classic CAN 用 d/r
    const bool is_rtr = (f.flags & 0x80u) != 0; // 你 Flags 里 RTR=(1<<7)
    const char* type = is_rtr ? "r" : "d";

    // DataLength：classic CAN 就等于 dlc（0..8）
    const int dlc = std::clamp<int>(static_cast<int>(f.dlc), 0, 8);
    const int data_len = is_rtr ? 0 : dlc;

    // MessageDuration / MessageLength：CanFrame 没有，先写 0（Vector 很多字段允许 0）
    const uint32_t msg_duration_ns = 0;
    const uint32_t msg_length_bits = 0;

    // Flags：给一个最小可用的 classic CAN flags（你没提供 Vector flags 体系，就保守输出 0）
    // 如果你后续要做得更真，可以映射：Tx/Rx、RTR、IDE 等到 Vector 的 flags bit。
    const uint32_t flags = 0;

    std::ostringstream oss;
    oss.setf(std::ios::fixed);
    oss << std::setprecision(6);

    // <Time> <Channel> <ID> <Dir> <Type> <DLC> <DataLength> <Data...> <MsgDuration> <MsgLength> <Flags>
    oss << t_sec << ' '
        << f.channel << ' ';

    // ID：hex，扩展帧加 'x'
    oss << std::hex << std::uppercase;
    if (asc_is_extended_id(f.id)) {
        oss << (f.id & 0x1FFFFFFFu) << 'x';
    } else {
        oss << (f.id & 0x7FFu);
    }
    oss << std::dec;

    // Dir
    oss << ' ' << asc_dir_from_flags(f.flags);

    // Type + DLC + DataLength
    oss << ' ' << type
        << ' ' << dlc
        << ' ' << data_len;

    // Data bytes（仅 data frame 输出）
    if (!is_rtr)
    {
        oss << std::hex << std::uppercase << std::setfill('0');
        for (int i = 0; i < dlc; ++i)
            oss << ' ' << std::setw(2) << static_cast<int>(f.data[i]);
        oss << std::dec;
    }

    // 结尾统计字段
    oss << ' ' << msg_duration_ns
        << ' ' << msg_length_bits
        << ' ' << std::hex << std::uppercase << flags
        << "\r\n";

    return oss.str();
}


bool CanMessageAscWriter::write(const BusMessage& msg, FileWriter& writer)
{
	const auto& can_msg = dynamic_cast<const CanMessage&>(msg);

	const auto& can_frame = can_msg.get_frame();

	auto line_msg = build_can_asc_line_full(
		can_frame,
		can_msg.get_timestamp(),
		writer.get_file_start_time(),
		false);

	writer.append(reinterpret_cast<const uint8_t*>(line_msg.data()), line_msg.size());

	return true;
}

}
