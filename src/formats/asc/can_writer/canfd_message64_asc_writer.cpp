#include "can/canfd_message64.h"
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

class CanFdMessage64AscWriter final : public IMessageWriter
{
public:
	bool write(const BusMessage& msg, FileWriter& writer) override
	{
		const auto& canfd_msg = dynamic_cast<const CanFdMessage64&>(msg);
		const auto& f = canfd_msg.get_frame();
		auto line = build_canfd64_asc_line(
			f,
			canfd_msg.get_timestamp(),
			writer.get_file_start_time());
		writer.append(reinterpret_cast<const uint8_t*>(line.data()), line.size());
		return true;
	}

	void set_timestamp_unit(TimeStampUnit unit) override
	{
		timestamp_unit_ = static_cast<uint32_t>(unit);
	}

private:
	static inline const char* asc_dir_from_dir(uint8_t dir)
	{
		return dir == 1 ? "Tx" : "Rx";
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

	static inline std::string build_canfd64_asc_line(
		const CanFdFrame64& f,
		uint64_t msg_timestamp_us,
		uint64_t file_start_time_us)
	{
		const uint64_t delta_us = msg_timestamp_us > file_start_time_us
			? msg_timestamp_us - file_start_time_us
			: 0ULL;
		const double t_sec = static_cast<double>(delta_us) / 1e6;
		const int brs = (f.flags & (1u << 13)) ? 1 : 0;
		const int esi = (f.flags & (1u << 14)) ? 1 : 0;
		uint8_t data_len = f.valid_data_bytes;
		if (data_len == 0) data_len = canfd_dlc_to_len(f.dlc);
		data_len = std::min<uint8_t>(data_len, 64);

		std::ostringstream oss;
		oss.setf(std::ios::fixed);
		oss << std::setprecision(6);

		oss << t_sec << " CANFD "
			<< static_cast<unsigned>(f.channel) << ' '
			<< asc_dir_from_dir(f.dir) << ' ';
		oss << std::hex << std::uppercase;
		if (asc_is_extended_id(f.id))
			oss << (f.id & 0x1FFFFFFFu) << 'x';
		else
			oss << (f.id & 0x7FFu);
		oss << std::dec << " 0 "
			<< brs << ' '
			<< esi << ' ';
		oss << std::hex << std::uppercase
			<< static_cast<int>(f.dlc)
			<< std::dec << ' '
			<< static_cast<int>(data_len);

		oss << std::hex << std::uppercase << std::setfill('0');
		for (int i = 0; i < static_cast<int>(data_len); ++i)
			oss << ' ' << std::setw(2) << static_cast<int>(f.data[i]);
		oss << std::dec;

		oss << ' ' << f.frame_length
			<< ' ' << f.bit_count
			<< ' ' << std::hex << std::uppercase
			<< f.flags << ' ' << 0 << ' ' << 0 << ' ' << 0 << ' ' << 0 << ' ' << 0
			<< "\r\n";
		return oss.str();
	}

	uint32_t timestamp_unit_{};
};

} // namespace

GWLOGGER_REGISTER_ASC_FRAME_WRITER(CanFdMessage64AscWriter, BusType::CAN_FD64)

} // namespace GWLogger::Asc
