#include "ethernet/ethernet_message.h"
#include "asc_frame_registration.h"
#include "asc_reader_helper.h"

#include <string>

namespace GWLogger::Asc
{

namespace
{

static bool parse_mac_token(const std::string& token, uint8_t address[6])
{
	size_t start = 0;
	for (int i = 0; i < 6; ++i)
	{
		const size_t end = token.find(':', start);
		const std::string part = token.substr(start, end == std::string::npos ? end : end - start);
		uint8_t value{};
		if (!parse_byte_hex2(part, value))
			return false;
		address[i] = value;

		if (i < 5)
		{
			if (end == std::string::npos) return false;
			start = end + 1;
		}
		else if (end != std::string::npos)
		{
			return false;
		}
	}
	return true;
}

static uint16_t dir_from_token(const std::string& dir)
{
	if (dir == "Tx") return 1;
	if (dir == "TxRq") return 2;
	return 0;
}

class EthernetFrameAscReader final : public IAscMessageReader
{
public:
	BusMessagePtr read_line(const std::string& line, uint64_t file_start_posix_us) const override
	{
		auto t = split_ws(line);
		if (t.size() < 11) return nullptr;
		if (t[1] != "ETHERNET") return nullptr;

		double time_sec = 0.0;
		if (!parse_double_strict(t[0], time_sec)) return nullptr;
		if (time_sec < 0) time_sec = 0;

		int ch = 0;
		if (!parse_int_strict(t[2], ch) || ch < 0 || ch > 0xFFFF) return nullptr;

		const std::string& dir = t[3];
		if (!(dir == "Tx" || dir == "Rx" || dir == "TxRq")) return nullptr;

		EthernetFrame frame{};
		frame.channel = static_cast<uint16_t>(ch);
		frame.dir = dir_from_token(dir);
		if (!parse_mac_token(t[4], frame.source_address)) return nullptr;
		if (!parse_mac_token(t[5], frame.destination_address)) return nullptr;

		uint32_t type = 0;
		if (!parse_u32_base_strict(t[6], 16, type) && !parse_u32_base_strict(t[6], 10, type)) return nullptr;
		if (type > 0xFFFF) return nullptr;
		frame.type = static_cast<uint16_t>(type);

		uint32_t tpid = 0;
		if (!parse_u32_base_strict(t[7], 16, tpid) && !parse_u32_base_strict(t[7], 10, tpid)) return nullptr;
		if (tpid > 0xFFFF) return nullptr;
		frame.tpid = static_cast<uint16_t>(tpid);

		uint32_t tci = 0;
		if (!parse_u32_base_strict(t[8], 16, tci) && !parse_u32_base_strict(t[8], 10, tci)) return nullptr;
		if (tci > 0xFFFF) return nullptr;
		frame.tci = static_cast<uint16_t>(tci);

		int payload_length = 0;
		if (!parse_int_strict(t[9], payload_length) ||
			payload_length < 0 ||
			payload_length > ETHERNET_MAX_PAYLOAD_LENGTH)
			return nullptr;

		if (t.size() < 10u + static_cast<size_t>(payload_length)) return nullptr;
		frame.payload_length = static_cast<uint16_t>(payload_length);

		for (int i = 0; i < payload_length; ++i)
		{
			uint8_t b{};
			if (!parse_byte_hex2(t[10 + i], b))
			{
				if (!parse_byte_dec(t[10 + i], b)) return nullptr;
			}
			frame.payload[i] = b;
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
		if (t.size() < 11) return false;
		if (t[1] != "ETHERNET") return false;
		if (!(t[3] == "Tx" || t[3] == "Rx" || t[3] == "TxRq")) return false;

		int payload_length = 0;
		if (!parse_int_strict(t[9], payload_length) ||
			payload_length < 0 ||
			payload_length > ETHERNET_MAX_PAYLOAD_LENGTH)
			return false;
		return t.size() >= 10u + static_cast<size_t>(payload_length);
	}
};

} // namespace

GWLOGGER_REGISTER_ASC_FRAME_READER(EthernetFrameAscReader, AscLineKey::Ethernet)

} // namespace GWLogger::Asc
