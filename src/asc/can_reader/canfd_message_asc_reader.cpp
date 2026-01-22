#include "canfd_message_asc_reader.h"
#include "blf_reader_registry.h"
#include "canfd_message.h"
#include "asc_reader_helper.h"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstdlib>
#include <string>
#include <vector>

namespace GWLogger::Asc
{

static AscReaderRegistrar<CanFdMessageAscReader> reg_canfd(AscLineKey::CanFd);


BusMessagePtr CanFdMessageAscReader::read_line(const std::string& line,
                                              uint64_t file_start_posix_us) const
{
    auto t = split_ws(line);
    if (t.size() < 3) return nullptr;

    // 过滤注释/空
    if (t[0].empty()) return nullptr;
    if (t[0].size() >= 2 && t[0][0] == '/' && t[0][1] == '/') return nullptr;

    // time
    double time_sec = 0.0;
    if (!parse_double_strict(t[0], time_sec)) return nullptr;
    if (time_sec < 0) time_sec = 0;

    // ====== 识别全量 CANFD 行：t[1] == "CANFD" ======
    if (t.size() >= 10 && t[1] == "CANFD")
    {
        // layout:
        // 0:<Time> 1:CANFD 2:<Channel> 3:<Dir> 4:<ID> 5:<Sym> 6:<BRS> 7:<ESI>
        // 8:<DLC> 9:<DataLength> 10..:<Data...> tail...

        int ch = 0;
        if (!parse_int_strict(t[2], ch) || ch < 0 || ch > 0xFFFF) return nullptr;

        const std::string& dir = t[3];

        uint32_t id = 0;
        if (!parse_can_id_token(t[4], id)) return nullptr;

        int brs = 0, esi = 0;
        if (!parse_int_strict(t[6], brs)) return nullptr;
        if (!parse_int_strict(t[7], esi)) return nullptr;

        // DLC：在你 writer 里是用 hex 输出（无0x），例如 A/F
        // 所以这里要按 base16 解析（也兼容 base10）
        uint32_t dlc_u32 = 0;
        if (!parse_u32_base_strict(t[8], 16, dlc_u32))
        {
            if (!parse_u32_base_strict(t[8], 10, dlc_u32)) return nullptr;
        }
        if (dlc_u32 > 15) return nullptr;
        const uint8_t dlc = static_cast<uint8_t>(dlc_u32);

        int data_len_i = 0;
        if (!parse_int_strict(t[9], data_len_i) || data_len_i < 0 || data_len_i > 64) return nullptr;
        uint8_t data_len = static_cast<uint8_t>(data_len_i);

        // data_len=0 的话，按 DLC 推导（兼容）
        if (data_len == 0) data_len = canfd_dlc_to_len(dlc);

        // data bytes 起点
        const size_t data_start = 10;
        const size_t tail_start = data_start + data_len;

        // tail 至少需要 9 个字段：
        // duration msglen flags crc btr_arb btr_data btr_ext_arb btr_ext_data
        // => tail_start + 8 < size
        if (t.size() < tail_start + 8) return nullptr;

        // parse data bytes (hex 2 chars)
        CanFdFrame f{};
        f.channel = static_cast<uint16_t>(ch);
        f.id      = id;
        f.dlc     = dlc;
        f.valid_data_bytes = data_len;

        // flags: 你现在 writer 用 flags==1/2 表示 Tx/Rx
        // 为了与现有系统兼容，这里仍填 1/2
        // 如果你之后要改成 bitfield（TX=1<<0），可以在这里改。
        if (dir == "Tx" || dir == "TxRq") f.flags = 1;
        else f.flags = 2;

        // can_fd_flags：把 BRS/ESI 写回去（你 enum: BRS=1<<1, ESI=1<<2）
        f.can_fd_flags = 0;
        if (brs) f.can_fd_flags |= (1u << 1);
        if (esi) f.can_fd_flags |= (1u << 2);
        // EDL(1<<0) 可选：你可以默认置 1 表示 FD
        f.can_fd_flags |= (1u << 0);

        // duration
        int duration_ns = 0;
        if (!parse_int_strict(t[tail_start + 0], duration_ns) || duration_ns < 0) duration_ns = 0;
        f.frame_length = static_cast<uint32_t>(duration_ns);

        // arb_bit_count / reserved 等：ASC 里 msglen 是 bits，但你的结构有 arb_bit_count(8-bit)
        // 这里先不强行塞，保留 0
        f.arb_bit_count = 0;

        // data
        for (size_t i = 0; i < data_len; ++i)
        {
            uint8_t b{};
            // Vector CANFD data 通常是两位 hex
            if (!parse_byte_hex2(t[data_start + i], b))
            {
                // 兼容 dec
                if (!parse_byte_dec(t[data_start + i], b)) return nullptr;
            }
            f.data[i] = b;
        }

        // timestamp: relative seconds -> posix_us
        const auto delta_us = static_cast<uint64_t>(time_sec * 1000000.0 + 0.5);
        const uint64_t posix_us = file_start_posix_us + delta_us;

        BusMessagePtr msg = make_message(f);
        if (!msg) return nullptr;
        msg->set_timestamp(posix_us);
        return msg;
    }

    // ====== 兼容：你以前的“简量 CANFD 行”（如果存在） ======
    // 例如： CANFD <Time> <Channel> <ID> <Dir> <DLC> <ValidBytes> <Data...>
    // 或其它简化格式 —— 你若确定不会再产生，可删掉这段。
    //
    // 这里给一个最简单的兼容：识别 t[0]=="CANFD" 的情况
    if (t[0] == "CANFD" && t.size() >= 7)
    {
        double ts2 = 0.0;
        if (!parse_double_strict(t[1], ts2)) return nullptr;
        if (ts2 < 0) ts2 = 0;

        int ch = 0;
        if (!parse_int_strict(t[2], ch) || ch < 0 || ch > 0xFFFF) return nullptr;

        uint32_t id = 0;
        if (!parse_can_id_token(t[3], id)) return nullptr;

        const std::string& dir = t[4];

        int dlc = 0;
        if (!parse_int_strict(t[5], dlc) || dlc < 0 || dlc > 15) return nullptr;

        int vbytes = 0;
        if (!parse_int_strict(t[6], vbytes) || vbytes < 0 || vbytes > 64) return nullptr;

        if (t.size() < 7u + static_cast<size_t>(vbytes)) return nullptr;

        CanFdFrame f{};
        f.channel = static_cast<uint16_t>(ch);
        f.id      = id;
        f.dlc     = static_cast<uint8_t>(dlc);
        f.valid_data_bytes = static_cast<uint8_t>(vbytes);
        f.flags = (dir == "Tx" || dir == "TxRq") ? 1 : 2;
        f.can_fd_flags = (1u << 0); // EDL

        for (int i = 0; i < vbytes; ++i)
        {
            uint8_t b{};
            if (!parse_byte_hex2(t[7 + i], b))
            {
                if (!parse_byte_dec(t[7 + i], b)) return nullptr;
            }
            f.data[i] = b;
        }

        const auto delta_us = static_cast<uint64_t>(ts2 * 1000000.0 + 0.5);
        const uint64_t posix_us = file_start_posix_us + delta_us;

        BusMessagePtr msg = make_message(f);
        if (!msg) return nullptr;
        msg->set_timestamp(posix_us);
        return msg;
    }

    return nullptr;
}

bool CanFdMessageAscReader::match(const std::string& line) const
{
    auto t = split_ws(line);

    // <Time> CANFD <Ch> <Dir> <ID> <Sym> <BRS> <ESI> <DLC> <DataLength> ...
    if (t.size() < 10) return false;
    if (t[1] != "CANFD") return false;

    // channel 必须是十进制
    int ch = 0;
    {
        const char* b = t[2].data();
        const char* e = t[2].data() + t[2].size();
        auto r = std::from_chars(b, e, ch, 10);
        if (r.ec != std::errc{} || r.ptr != e) return false;
        if (ch < 0 || ch > 0xFFFF) return false;
    }

    // dir
    if (!(t[3] == "Tx" || t[3] == "Rx" || t[3] == "TxRq")) return false;

    // DLC：你 writer 输出的是 hex (0..F)
    uint32_t dlc_u32 = 0;
    {
        // strtoul 更好处理 A/F 这种
        char* endp = nullptr;
        unsigned long v = std::strtoul(t[8].c_str(), &endp, 16);
        if (endp == t[8].c_str() || *endp != '\0') return false;
        if (v > 15) return false;
        dlc_u32 = static_cast<uint32_t>(v);
    }

    // DataLength：十进制 0..64
    int data_len = 0;
    {
        const char* b = t[9].data();
        const char* e = t[9].data() + t[9].size();
        auto r = std::from_chars(b, e, data_len, 10);
        if (r.ec != std::errc{} || r.ptr != e) return false;
        if (data_len < 0 || data_len > 64) return false;
    }

    // data bytes 至少要有 data_len 个
    if (static_cast<int>(t.size()) < 10 + data_len) return false;

    return true;
}


}