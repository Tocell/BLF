#include "lin/lin_message.h"
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

class LinMessageAscWriter final : public IMessageWriter
{
public:
	bool write(const BusMessage& msg, FileWriter& writer) override
	{
		const auto& lin_msg = dynamic_cast<const LinMessage&>(msg);
		const auto& frame = lin_msg.get_frame();
		auto line = build_lin_asc_line(
			frame,
			lin_msg.get_timestamp(),
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
	static inline const char* asc_dir_from_lin_dir(uint8_t dir)
	{
		return dir == 1 ? "Tx" : "Rx";
	}

	static inline std::string build_lin_asc_line(
		const LinFrame& frame,
		uint64_t msg_timestamp_us,
		uint64_t file_start_time_us,
		bool timestamps_absolute)
	{
		double t_sec = 0.0;
		if (timestamps_absolute) {
			t_sec = static_cast<double>(msg_timestamp_us) / 1e6;
		} else {
			const uint64_t base = std::min(msg_timestamp_us, file_start_time_us);
			t_sec = static_cast<double>(msg_timestamp_us - base) / 1e6;
		}

		const int dlc = std::clamp<int>(static_cast<int>(frame.dlc), 0, 8);

		std::ostringstream oss;
		oss.setf(std::ios::fixed);
		oss << std::setprecision(6);
		oss << t_sec << " LIN "
			<< frame.channel << ' '
			<< asc_dir_from_lin_dir(frame.dir) << ' '
			<< std::hex << std::uppercase << static_cast<int>(frame.id)
			<< std::dec << ' '
			<< dlc;

		oss << std::hex << std::uppercase << std::setfill('0');
		for (int i = 0; i < dlc; ++i)
			oss << ' ' << std::setw(2) << static_cast<int>(frame.data[i]);

		oss << std::dec << ' ' << frame.crc << "\r\n";
		return oss.str();
	}

	uint32_t timestamp_unit_{};
};

} // namespace

GWLOGGER_REGISTER_ASC_FRAME_WRITER(LinMessageAscWriter, BusType::LIN)

} // namespace GWLogger::Asc
