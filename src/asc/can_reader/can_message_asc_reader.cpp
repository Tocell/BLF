#include "can_message_asc_reader.h"

#include "blf_reader_registry.h"
#include "can_message.h"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstdlib>
#include <string>
#include <vector>

namespace GWLogger::Asc
{

static AscReaderRegistrar<CanMessageAscReader> reg_can;

uint32_t CanMessageAscReader::key() const
{
    return kKey;
}

static inline std::vector<std::string> split_ws(const std::string& s)
{
    std::vector<std::string> out;
    std::string cur;
    cur.reserve(16);

    for (unsigned char ch : s)
    {
        if (std::isspace(ch))
        {
            if (!cur.empty())
            {
                out.push_back(std::move(cur));
                cur.clear();
                cur.reserve(16);
            }
        }
        else
        {
            cur.push_back(static_cast<char>(ch));
        }
    }
    if (!cur.empty())
        out.push_back(std::move(cur));
    return out;
}

static inline bool parse_double_strict(const std::string& s, double& out)
{
    // strtod 容忍度较高，但这里用 endptr 保证全串吃完
    char* endp = nullptr;
    out = std::strtod(s.c_str(), &endp);
    return endp && *endp == '\0';
}

static inline bool parse_int_strict(const std::string& s, int& out)
{
    const char* b = s.data();
    const char* e = s.data() + s.size();
    auto r = std::from_chars(b, e, out, 10);
    return r.ec == std::errc{} && r.ptr == e;
}

static inline bool parse_u32_base_strict(const std::string& s, int base, uint32_t& out)
{
    // base=10 or 16
    char* endp = nullptr;
    unsigned long v = std::strtoul(s.c_str(), &endp, base);
    if (endp == s.c_str() || *endp != '\0') return false;
    if (v > 0xFFFFFFFFul) return false;
    out = static_cast<uint32_t>(v);
    return true;
}

static inline bool parse_byte_hex2(const std::string& s, uint8_t& out)
{
    if (s.size() != 2) return false;
    auto hex = [](char c)->int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
        if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
        return -1;
    };
    int hi = hex(s[0]), lo = hex(s[1]);
    if (hi < 0 || lo < 0) return false;
    out = static_cast<uint8_t>((hi << 4) | lo);
    return true;
}

static inline bool parse_byte_dec(const std::string& s, uint8_t& out)
{
    int v = 0;
    if (!parse_int_strict(s, v)) return false;
    if (v < 0 || v > 255) return false;
    out = static_cast<uint8_t>(v);
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