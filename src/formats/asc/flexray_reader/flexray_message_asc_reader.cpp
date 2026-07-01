#include "flexray/flexray_message.h"
#include "asc_frame_registration.h"
#include "asc_reader_helper.h"

#include <string>

namespace GWLogger::Asc
{

namespace
{

class FlexRayMessageAscReader final : public IAscMessageReader
{
public:
	BusMessagePtr read_line(const std::string& line, uint64_t file_start_posix_us) const override
	{
		auto t = split_ws(line);
		if (t.size() < 9) return nullptr;
		if (t[1] != "FLEXRAY") return nullptr;

		double time_sec = 0.0;
		if (!parse_double_strict(t[0], time_sec)) return nullptr;
		if (time_sec < 0) time_sec = 0;

		int ch = 0;
		if (!parse_int_strict(t[2], ch) || ch < 0 || ch > 0xFFFF) return nullptr;

		const std::string& dir = t[3];
		if (!(dir == "Tx" || dir == "Rx" || dir == "TxRq")) return nullptr;

		int frame_id = 0;
		if (!parse_int_strict(t[4], frame_id) || frame_id < 0 || frame_id > 0xFFFF) return nullptr;

		int cycle = 0;
		if (!parse_int_strict(t[5], cycle) || cycle < 0 || cycle > 0xFF) return nullptr;

		int data_len = 0;
		if (!parse_int_strict(t[6], data_len) || data_len < 0 || data_len > 64) return nullptr;

		int header_crc = 0;
		if (!parse_int_strict(t[7], header_crc) || header_crc < 0 || header_crc > 0xFFFF) return nullptr;

		int frame_state = 0;
		if (!parse_int_strict(t[8], frame_state) || frame_state < 0 || frame_state > 0xFFFF) return nullptr;

		if (t.size() < 9u + static_cast<size_t>(data_len)) return nullptr;

		FlexRayFrame frame{};
		frame.channel = static_cast<uint16_t>(ch);
		frame.dir = (dir == "Tx" || dir == "TxRq") ? 1 : 0;
		frame.frame_id = static_cast<uint16_t>(frame_id);
		frame.cycle = static_cast<uint8_t>(cycle);
		frame.length = static_cast<uint8_t>(data_len);
		frame.header_crc = static_cast<uint16_t>(header_crc);
		frame.frame_state = static_cast<uint16_t>(frame_state);

		for (int i = 0; i < data_len; ++i)
		{
			uint8_t b{};
			if (!parse_byte_hex2(t[9 + i], b))
			{
				if (!parse_byte_dec(t[9 + i], b)) return nullptr;
			}
			frame.data[i] = b;
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
		if (t.size() < 9) return false;
		if (t[1] != "FLEXRAY") return false;
		if (!(t[3] == "Tx" || t[3] == "Rx" || t[3] == "TxRq")) return false;

		int data_len = 0;
		if (!parse_int_strict(t[6], data_len) || data_len < 0 || data_len > 64) return false;
		return t.size() >= 9u + static_cast<size_t>(data_len);
	}
};

} // namespace

GWLOGGER_REGISTER_ASC_FRAME_READER(FlexRayMessageAscReader, AscLineKey::FlexRay)

} // namespace GWLogger::Asc
