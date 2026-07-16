#include "can/canfd_message.h"
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

class CanFdMessageAscWriter final : public IMessageWriter
{
public:
	bool write(const BusMessage& msg, FileWriter& writer) override
	{
		const auto& canfd_msg = dynamic_cast<const CanFdMessage&>(msg);
		const auto& f = canfd_msg.get_frame();
		auto line = build_canfd_asc_line(
			f,
			canfd_msg.get_timestamp(),
			writer.get_file_start_time(),
			false);
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
		if (flags == 1) return "Tx";
		if (flags == 2) return "Rx";
		return "Rx";
	}

	static inline bool asc_is_extended_id(uint32_t id)
	{
		return id > 0x7FFu;
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

	static inline std::string build_canfd_asc_line(
		const CanFdFrame& f,
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

		const int brs = (f.can_fd_flags & (1u << 1)) ? 1 : 0;
		const int esi = (f.can_fd_flags & (1u << 2)) ? 1 : 0;
		uint8_t data_len = f.valid_data_bytes;
		if (data_len == 0) data_len = canfd_dlc_to_len(f.dlc);
		data_len = std::min<uint8_t>(data_len, 64);
		const uint32_t msg_duration_ns = f.frame_length;
		const uint32_t msg_length_bits = 0;
		uint32_t flags = 0;
		flags |= (1u << 12);
		if (brs) flags |= (1u << 13);
		if (esi) flags |= (1u << 14);

		std::ostringstream oss;
		oss.setf(std::ios::fixed);
		oss << std::setprecision(6);

		oss << t_sec << " CANFD "
			<< f.channel << ' '
			<< asc_dir_from_flags(f.flags) << ' ';
		oss << std::hex << std::uppercase;
		if (asc_is_extended_id(f.id)) {
			oss << (f.id & 0x1FFFFFFFu) << 'x';
		} else {
			oss << (f.id & 0x7FFu);
		}
		oss << std::dec << " 0 "
			<< brs << ' '
			<< esi << ' ';
		oss << std::hex << std::uppercase
			<< static_cast<int>(f.dlc)
			<< std::dec << ' ';
		oss << static_cast<int>(data_len);
		oss << std::hex << std::uppercase << std::setfill('0');
		for (int i = 0; i < static_cast<int>(data_len); ++i)
			oss << ' ' << std::setw(2) << static_cast<int>(f.data[i]);
		oss << std::dec;
		oss << ' ' << msg_duration_ns
			<< ' ' << msg_length_bits
			<< ' ' << std::hex << std::uppercase
			<< flags << ' ' << 0 << ' ' << 0 << ' ' << 0 << ' ' << 0 << ' ' << 0
			<< "\r\n";

		return oss.str();
	}

	uint32_t timestamp_unit_{};
};

} // namespace

GWLOGGER_REGISTER_ASC_FRAME_WRITER(CanFdMessageAscWriter, BusType::CAN_FD)

} // namespace GWLogger::Asc
