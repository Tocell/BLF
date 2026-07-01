#ifndef ASC_READER_HELPER_H
#define ASC_READER_HELPER_H
#include <cstdint>
#include <string>
#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstdlib>
#include <vector>

namespace GWLogger::Asc
{

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

static inline bool parse_can_id_token(const std::string& tok, uint32_t& out_id)
{
    std::string s = tok;
    if (!s.empty() && (s.back() == 'x' || s.back() == 'X'))
        s.pop_back();

    if (parse_u32_base_strict(s, 16, out_id)) return true;
    if (parse_u32_base_strict(s, 10, out_id)) return true;
    return false;
}

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


}



#endif //ASC_READER_HELPER_H
