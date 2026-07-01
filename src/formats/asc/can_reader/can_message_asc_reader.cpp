#include "can/can_message.h"
#include "asc_frame_registration.h"
#include "asc_reader_helper.h"

#include <charconv>
#include <string>
#include <vector>

namespace GWLogger::Asc
{

namespace
{

class CanMessageAscReader final : public IAscMessageReader
{
public:
	BusMessagePtr read_line(const std::string& line, uint64_t file_start_posix_us) const override
	{
		auto t = split_ws(line);
		if (t.size() < 6) return nullptr;

		double time_sec = 0.0;
		if (!parse_double_strict(t[0], time_sec)) return nullptr;
		if (time_sec < 0) time_sec = 0;

		int ch = 0;
		if (!parse_int_strict(t[1], ch) || ch < 0 || ch > 0xFFFF) return nullptr;

		uint32_t id = 0;
		if (!parse_can_id_token(t[2], id)) return nullptr;

		const std::string& dir = t[3];
		const std::string& type = t[4];
		if (!(type == "d" || type == "r")) return nullptr;

		int dlc = 0;
		if (!parse_int_strict(t[5], dlc) || dlc < 0 || dlc > 8) return nullptr;

		CanFrame f{};
		f.channel = static_cast<uint16_t>(ch);
		f.dlc = static_cast<uint8_t>(dlc);
		f.id = id;
		f.flags = (dir == "Tx" || dir == "TxRq") ? 1 : 2;
		if (type == "r")
			f.flags = static_cast<uint8_t>(f.flags | 0x80u);

		bool parsed_full = false;
		if (t.size() >= 10)
		{
			int tmp_data_len = 0;
			if (parse_int_strict(t[6], tmp_data_len) && tmp_data_len >= 0 && tmp_data_len <= 8)
			{
				const size_t data_start = 7;
				const size_t need_data = (type == "d") ? static_cast<size_t>(tmp_data_len) : 0u;
				const size_t tail_start = data_start + need_data;
				if (t.size() >= tail_start + 3)
				{
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

					int msg_duration_ns = 0;
					int msg_length_bits = 0;
					uint32_t flags_hex = 0;
					if (!parse_int_strict(t[tail_start + 0], msg_duration_ns)) return nullptr;
					if (!parse_int_strict(t[tail_start + 1], msg_length_bits)) return nullptr;
					if (!parse_u32_base_strict(t[tail_start + 2], 16, flags_hex))
					{
						if (!parse_u32_base_strict(t[tail_start + 2], 10, flags_hex)) return nullptr;
					}
					(void)msg_duration_ns;
					(void)msg_length_bits;
					(void)flags_hex;
					parsed_full = true;
				}
			}
		}

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

		const auto delta_us = static_cast<uint64_t>(time_sec * 1000000.0 + 0.5);
		const uint64_t posix_us = file_start_posix_us + delta_us;

		BusMessagePtr msg = make_message(f);
		if (!msg) return nullptr;
		msg->set_timestamp(posix_us);
		return msg;
	}

	bool match(const std::string& line) const override
	{
		auto t = split_ws(line);
		if (t.size() < 6) return false;

		int ch = 0;
		{
			const char* b = t[1].data();
			const char* e = t[1].data() + t[1].size();
			auto r = std::from_chars(b, e, ch, 10);
			if (r.ec != std::errc{} || r.ptr != e) return false;
		}

		if (!(t[3] == "Tx" || t[3] == "Rx" || t[3] == "TxRq")) return false;
		if (!(t[4] == "d" || t[4] == "r")) return false;

		int dlc = 0;
		{
			const char* b = t[5].data();
			const char* e = t[5].data() + t[5].size();
			auto r = std::from_chars(b, e, dlc, 10);
			if (r.ec != std::errc{} || r.ptr != e) return false;
			if (dlc < 0 || dlc > 8) return false;
		}

		if (t[4] == "d" && static_cast<int>(t.size()) < 6 + dlc) return false;
		return true;
	}
};

} // namespace

GWLOGGER_REGISTER_ASC_FRAME_READER(CanMessageAscReader, AscLineKey::CanClassic)

} // namespace GWLogger::Asc
