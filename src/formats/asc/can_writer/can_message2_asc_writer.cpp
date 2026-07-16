#include "can/can_message2.h"
#include "asc_frame_registration.h"

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

namespace GWLogger::Asc
{

namespace
{

class CanMessage2AscWriter final : public IMessageWriter
{
public:
	bool write(const BusMessage& msg, FileWriter& writer) override
	{
		const auto& can_msg = dynamic_cast<const CanMessage2&>(msg);
		const auto& f2 = can_msg.get_frame();

		CanFrame f{};
		f.channel = f2.channel;
		f.flags = f2.flags;
		f.dlc = f2.dlc;
		f.id = f2.id;
		std::copy_n(f2.data, 8, f.data);

		auto line = build_can_asc_line(
			f,
			f2.frame_length,
			f2.bit_count,
			can_msg.get_timestamp(),
			writer.get_file_start_time());
		writer.append(reinterpret_cast<const uint8_t*>(line.data()), line.size());
		return true;
	}

	void set_timestamp_unit(TimeStampUnit unit) override
	{
		timestamp_unit_ = static_cast<uint32_t>(unit);
	}

private:
	static inline const char* asc_dir_from_flags(uint8_t flags)
	{
		if ((flags & 0x0Fu) == 1) return "Tx";
		return "Rx";
	}

	static inline bool asc_is_extended_id(uint32_t id)
	{
		return id > 0x7FFu;
	}

	static inline std::string build_can_asc_line(
		const CanFrame& f,
		uint32_t msg_duration_ns,
		uint32_t msg_length_bits,
		uint64_t msg_timestamp_us,
		uint64_t file_start_time_us)
	{
		const uint64_t delta_us = msg_timestamp_us > file_start_time_us
			? msg_timestamp_us - file_start_time_us
			: 0ULL;
		const double t_sec = static_cast<double>(delta_us) / 1e6;
		const bool is_rtr = (f.flags & 0x80u) != 0;
		const char* type = is_rtr ? "r" : "d";
		const int dlc = std::clamp<int>(static_cast<int>(f.dlc), 0, 8);
		const int data_len = is_rtr ? 0 : dlc;
		const uint32_t flags = 0;

		std::ostringstream oss;
		oss.setf(std::ios::fixed);
		oss << std::setprecision(6);

		oss << t_sec << ' ' << f.channel << ' ';
		oss << std::hex << std::uppercase;
		if (asc_is_extended_id(f.id))
			oss << (f.id & 0x1FFFFFFFu) << 'x';
		else
			oss << (f.id & 0x7FFu);
		oss << std::dec << ' ' << asc_dir_from_flags(f.flags)
			<< ' ' << type << ' ' << dlc << ' ' << data_len;

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

GWLOGGER_REGISTER_ASC_FRAME_WRITER(CanMessage2AscWriter, BusType::CAN2)

} // namespace GWLogger::Asc
