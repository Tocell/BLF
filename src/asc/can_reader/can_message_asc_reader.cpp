#include "can_message_asc_reader.h"

#include "../../registry/reader_registrar.h"
#include "../../registry/reader_registry.h"
#include "../../object/can/can_message.h"
#include "../asc_object_type_manager.h"
#include "asc_praser_line_helper.h"

#include <cmath>

namespace BLF
{

static AscReaderRegistrar<CanMessageAscReader> reg_can;

bool CanMessageAscReader::match(const std::string& line) const
{
    if (line.empty()) return false;
    if (line.size() >= 2 && line[0] == '/' && line[1] == '/') return false;

    auto t = split_ws(line);
    if (t.size() < 6) return false;

    // dir + d/r
    if (!(t[3] == "Tx" || t[3] == "Rx" || t[3] == "TxRq")) return false;
    if (!(t[4] == "d"  || t[4] == "r")) return false;

    // time/channel/dlc 基本合法性
    double ts = 0.0;
    if (!parse_double_strict(t[0], ts)) return false;

    int ch = 0;
    if (!parse_int_strict(t[1], ch)) return false;

    int dlc = 0;
    if (!parse_int_strict(t[5], dlc) || dlc < 0 || dlc > 8) return false;

    if (t[4] == "d" && (int)t.size() < 6 + dlc) return false;
    return true;
}

BusMessagePtr CanMessageAscReader::read_line(const std::string& line,
                        uint64_t file_start_posix_us) const
{
    auto t = split_ws(line);
    if (t.size() < 6) return nullptr;

    double time_sec = 0.0;
    if (!parse_double_strict(t[0], time_sec)) return nullptr;

    int ch = 0;
    if (!parse_int_strict(t[1], ch) || ch < 0 || ch > 0xFFFF) return nullptr;

    // 先按 hex 解析（因为你写 header 是 base hex），不行再 fallback dec
    uint32_t id = 0;
    if (!parse_u32_base_strict(t[2], 16, id))
    {
        if (!parse_u32_base_strict(t[2], 10, id)) return nullptr;
    }

    const std::string& dir = t[3];
    const std::string& dr  = t[4];

    int dlc = 0;
    if (!parse_int_strict(t[5], dlc) || dlc < 0 || dlc > 8) return nullptr;

    CanFrame f{};
    f.channel = static_cast<uint16_t>(ch);
    f.dlc     = static_cast<uint8_t>(dlc);
    f.id      = id;

    // flags: 1=Tx 2=Rx （你定义）
    if (dir == "Tx" || dir == "TxRq") f.flags = 1;
    else f.flags = 2;

    // remote frame (r) 一般不带 data，这里只在 d 的情况下解析 data
    if (dr == "d")
    {
        if (static_cast<int>(t.size()) < 6 + dlc) return nullptr;

        for (int i = 0; i < dlc; ++i)
        {
            uint8_t b{};
            // 优先按 hex byte 解析；失败再按 dec
            if (!parse_byte_hex2(t[6 + i], b))
            {
                if (!parse_byte_dec(t[6 + i], b)) return nullptr;
            }
            f.data[i] = b;
        }
    }
    else
    {
        // remote frame：可按需打一个标志到 flags（如果你后续需要区分）
        // 当前 CanFrame 没有字段表示 remote，这里先不处理。
    }

    // 还原时间戳：样例是相对秒，所以 posix_us = file_start + delta_us
    // 这里做个保护：time_sec 可能很大/负数（理论上不该），做 clamp
    if (time_sec < 0) time_sec = 0;

    const uint64_t delta_us = static_cast<uint64_t>(time_sec * 1000000.0 + 0.5);
    const uint64_t posix_us = file_start_posix_us + delta_us;

    // 生成消息（你已有 MessageType<CanFrame>::type = CanMessage）
    BusMessagePtr msg = make_message(f);
    if (!msg) return nullptr;
    msg->set_timestamp(posix_us);
    return msg;
}

}