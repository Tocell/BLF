#include "can/can_message.h"
#include "asc_frame_registration.h"

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

#include "imessage_writer.h"

namespace GWLogger::Asc
{

namespace
{

class CanMessageAscWriter final : public IMessageWriter
{
public:
	bool write(const BusMessage& msg, FileWriter& writer) override
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

	void set_timestamp_unit(TimeStampUnit unit) override
	{
		timestamp_unit_ = static_cast<uint32_t>(unit);
	}

private:
	static inline const char* asc_dir_from_flags(uint8_t flags)
	{
		if (flags == 1) return "Tx";
		if (flags == 2) return "Rx";
		return "Rx";
	}

	static inline bool asc_is_extended_id(uint32_t id)
	{
		return id > 0x7FFu;
	}

	static inline std::string build_can_asc_line_full(
		const CanFrame& f,
		uint64_t msg_timestamp_us,
		uint64_t file_start_time_us,
		bool timestamps_absolute)
	{
		double t_sec = 0.0;
		if (timestamps_absolute) {
			t_sec = static_cast<double>(msg_timestamp_us) / 1e6;
		} else {
			const uint64_t delta_us = msg_timestamp_us > file_start_time_us
				? msg_timestamp_us - file_start_time_us
				: 0ULL;
			t_sec = static_cast<double>(delta_us) / 1e6;
		}

		const bool is_rtr = (f.flags & 0x80u) != 0;
		const char* type = is_rtr ? "r" : "d";
		const int dlc = std::clamp<int>(static_cast<int>(f.dlc), 0, 8);
		const int data_len = is_rtr ? 0 : dlc;
		const uint32_t msg_duration_ns = 0;
		const uint32_t msg_length_bits = 0;
		const uint32_t flags = 0;

		std::ostringstream oss;
		oss.setf(std::ios::fixed);
		oss << std::setprecision(6);

		oss << t_sec << ' ' << f.channel << ' ';
		oss << std::hex << std::uppercase;
		if (asc_is_extended_id(f.id)) {
			oss << (f.id & 0x1FFFFFFFu) << 'x';
		} else {
			oss << (f.id & 0x7FFu);
		}
		oss << std::dec;
		oss << ' ' << asc_dir_from_flags(f.flags);
		oss << ' ' << type << ' ' << dlc << ' ' << data_len;

		if (!is_rtr)
		{
			oss << std::hex << std::uppercase << std::setfill('0');
			for (int i = 0; i < dlc; ++i)
				oss << ' ' << std::setw(2) << static_cast<int>(f.data[i]);
			oss << std::dec;
		}

		oss << ' ' << msg_duration_ns
			<< ' ' << msg_length_bits
			<< ' ' << std::hex << std::uppercase << flags
			<< "\r\n";

		return oss.str();
	}

	uint32_t timestamp_unit_{};
};

} // namespace

GWLOGGER_REGISTER_ASC_FRAME_WRITER(CanMessageAscWriter, BusType::CAN)

} // namespace GWLogger::Asc
