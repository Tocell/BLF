#include "lin/lin_message.h"
#include "asc_frame_registration.h"
#include "asc_reader_helper.h"

#include <string>

namespace GWLogger::Asc
{

namespace
{

class LinMessageAscReader final : public IAscMessageReader
{
public:
	BusMessagePtr read_line(const std::string& line, uint64_t file_start_posix_us) const override
	{
		auto t = split_ws(line);
		if (t.size() < 7) return nullptr;
		if (t[1] != "LIN") return nullptr;

		double time_sec = 0.0;
		if (!parse_double_strict(t[0], time_sec)) return nullptr;
		if (time_sec < 0) time_sec = 0;

		int ch = 0;
		if (!parse_int_strict(t[2], ch) || ch < 0 || ch > 0xFFFF) return nullptr;

		const std::string& dir = t[3];
		if (!(dir == "Tx" || dir == "Rx" || dir == "TxRq")) return nullptr;

		uint32_t id = 0;
		if (!parse_can_id_token(t[4], id) || id > 0xFF) return nullptr;

		int dlc = 0;
		if (!parse_int_strict(t[5], dlc) || dlc < 0 || dlc > 8) return nullptr;
		if (t.size() < 6u + static_cast<size_t>(dlc)) return nullptr;

		LinFrame frame{};
		frame.channel = static_cast<uint16_t>(ch);
		frame.id = static_cast<uint8_t>(id);
		frame.dlc = static_cast<uint8_t>(dlc);
		frame.dir = (dir == "Tx" || dir == "TxRq") ? 1 : 0;

		for (int i = 0; i < dlc; ++i)
		{
			uint8_t b{};
			if (!parse_byte_hex2(t[6 + i], b))
			{
				if (!parse_byte_dec(t[6 + i], b)) return nullptr;
			}
			frame.data[i] = b;
		}

		if (t.size() > 6u + static_cast<size_t>(dlc))
		{
			int crc = 0;
			if (!parse_int_strict(t[6 + dlc], crc) || crc < 0 || crc > 0xFFFF) return nullptr;
			frame.crc = static_cast<uint16_t>(crc);
		}

		const auto delta_us = static_cast<uint64_t>(time_sec * 1000000.0 + 0.5);
		const uint64_t posix_us = file_start_posix_us + delta_us;

		BusMessagePtr msg = make_message(frame);
		if (!msg) return nullptr;
		msg->set_timestamp(posix_us);
		return msg;
	}

	bool match(const std::string& line) const override
	{
		auto t = split_ws(line);
		if (t.size() < 7) return false;
		if (t[1] != "LIN") return false;
		if (!(t[3] == "Tx" || t[3] == "Rx" || t[3] == "TxRq")) return false;

		int ch = 0;
		if (!parse_int_strict(t[2], ch) || ch < 0 || ch > 0xFFFF) return false;

		uint32_t id = 0;
		if (!parse_can_id_token(t[4], id) || id > 0xFF) return false;

		int dlc = 0;
		if (!parse_int_strict(t[5], dlc) || dlc < 0 || dlc > 8) return false;
		return t.size() >= 6u + static_cast<size_t>(dlc);
	}
};

} // namespace

GWLOGGER_REGISTER_ASC_FRAME_READER(LinMessageAscReader, AscLineKey::Lin)

} // namespace GWLogger::Asc
