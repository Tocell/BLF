#include "can_message_asc_reader.h"
#include "blf_reader_registry.h"
#include "can_message.h"
#include "asc_reader_helper.h"

#include <charconv>
#include <string>
#include <vector>

namespace GWLogger::Asc
{

static AscReaderRegistrar<CanMessageAscReader>   reg_can(AscLineKey::CanClassic);



BusMessagePtr CanMessageAscReader::read_line(const std::string& line,
                                            uint64_t file_start_posix_us) const
{
    auto t = split_ws(line);
    if (t.size() < 6) return nullptr;

    // time
    double time_sec = 0.0;
    if (!parse_double_strict(t[0], time_sec)) return nullptr;
    if (time_sec < 0) time_sec = 0;

    // channel
    int ch = 0;
    if (!parse_int_strict(t[1], ch) || ch < 0 || ch > 0xFFFF) return nullptr;

    // id (支持 x)
    uint32_t id = 0;
    if (!parse_can_id_token(t[2], id)) return nullptr;

    // dir
    const std::string& dir = t[3];

    // type: d / r
    const std::string& type = t[4];
    if (!(type == "d" || type == "r")) return nullptr;

    // dlc
    int dlc = 0;
    if (!parse_int_strict(t[5], dlc) || dlc < 0 || dlc > 8) return nullptr;

    // 构造 CanFrame
    CanFrame f{};
    f.channel = static_cast<uint16_t>(ch);
    f.dlc     = static_cast<uint8_t>(dlc);
    f.id      = id;

    // 你的 flags 约定：1=Tx 2=Rx（但 remote/bitfield 建议加 RTR 位）
    if (dir == "Tx" || dir == "TxRq") f.flags = 1;
    else f.flags = 2;

    if (type == "r")
    {
        f.flags = static_cast<uint8_t>(f.flags | 0x80u);
    }

    // =========================
    // 尝试按“全量格式”解析
    // full: time ch id dir type dlc dataLen [data...] msgDuration msgLength flags
    // =========================
    bool parsed_full = false;
    int data_len = 0;

    // full 至少需要 dataLen + 3 个尾字段，所以 tokens >= 7 + 3 = 10（即使 dataLen=0）
    if (t.size() >= 10)
    {
        int tmp_data_len = 0;
        if (parse_int_strict(t[6], tmp_data_len) && tmp_data_len >= 0 && tmp_data_len <= 8)
        {
            const size_t data_start = 7;
            const size_t need_data = (type == "d") ? static_cast<size_t>(tmp_data_len) : 0u;

            // 尾字段起点
            const size_t tail_start = data_start + need_data;

            // tail_start + 3 个字段（duration/msglen/flags）必须存在
            if (t.size() >= tail_start + 3)
            {
                // 解析数据
                if (type == "d")
                {
                    for (size_t i = 0; i < need_data; ++i)
                    {
                        uint8_t b{};
                        if (!parse_byte_hex2(t[data_start + i], b))
                        {
                            if (!parse_byte_dec(t[data_start + i], b)) return nullptr;
                        }
                        f.data[i] = b;
                    }
                }

                // duration/msglen/flags（你暂时不用也没关系，但这里把格式吃掉）
                int msg_duration_ns = 0;
                int msg_length_bits = 0;
                uint32_t flags_hex = 0;

                // duration/msglen 是十进制
                if (!parse_int_strict(t[tail_start + 0], msg_duration_ns)) return nullptr;
                if (!parse_int_strict(t[tail_start + 1], msg_length_bits)) return nullptr;

                if (!parse_u32_base_strict(t[tail_start + 2], 16, flags_hex))
                {
                    if (!parse_u32_base_strict(t[tail_start + 2], 10, flags_hex)) return nullptr;
                }

                data_len = tmp_data_len;
                parsed_full = true;
            }
        }
    }

    // =========================
    // fallback：简量格式
    // simple: time ch id dir type dlc data...
    // =========================
    if (!parsed_full)
    {
        if (type == "d")
        {
            if (t.size() < 6u + static_cast<size_t>(dlc))
                return nullptr;

            for (int i = 0; i < dlc; ++i)
            {
                uint8_t b{};
                if (!parse_byte_hex2(t[6 + i], b))
                {
                    if (!parse_byte_dec(t[6 + i], b)) return nullptr;
                }
                f.data[i] = b;
            }
        }
    }

    // time(sec) -> posix_us
    const auto delta_us = static_cast<uint64_t>(time_sec * 1000000.0 + 0.5);
    const uint64_t posix_us = file_start_posix_us + delta_us;

    BusMessagePtr msg = make_message(f);
    if (!msg) return nullptr;

    msg->set_timestamp(posix_us);
    return msg;
}

bool CanMessageAscReader::match(const std::string& line) const
{
    auto t = split_ws(line);
    // classic CAN: <Time> <Ch> <ID> <Dir> <d/r> <DLC> ...
    if (t.size() < 6) return false;

    // 第2列必须是 channel 数字
    // （如果是 CANFD 行，t[1] 会是 "CANFD"，这里会直接失败）
    int ch = 0;
    {
        const char* b = t[1].data();
        const char* e = t[1].data() + t[1].size();
        auto r = std::from_chars(b, e, ch, 10);
        if (r.ec != std::errc{} || r.ptr != e) return false;
    }

    // 第4列 Tx/Rx/TxRq，第5列 d/r
    if (!(t[3] == "Tx" || t[3] == "Rx" || t[3] == "TxRq")) return false;
    if (!(t[4] == "d"  || t[4] == "r")) return false;

    // DLC 必须是十进制 0..8（classic CAN）
    int dlc = 0;
    {
        const char* b = t[5].data();
        const char* e = t[5].data() + t[5].size();
        auto r = std::from_chars(b, e, dlc, 10);
        if (r.ec != std::errc{} || r.ptr != e) return false;
        if (dlc < 0 || dlc > 8) return false;
    }

    // data frame 至少要有 dlc 个字节
    if (t[4] == "d" && static_cast<int>(t.size()) < 6 + dlc) return false;

    return true;
}


}