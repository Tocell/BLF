#include "flexray/flexray_message.h"
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

class FlexRayMessageAscWriter final : public IMessageWriter
{
public:
	bool write(const BusMessage& msg, FileWriter& writer) override
	{
		const auto& flexray_msg = dynamic_cast<const FlexRayMessage&>(msg);
		const auto& frame = flexray_msg.get_frame();
		auto line = build_flexray_asc_line(
			frame,
			flexray_msg.get_timestamp(),
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
	static inline const char* asc_dir_from_flexray_dir(uint8_t dir)
	{
		return dir == 1 ? "Tx" : "Rx";
	}

	static inline std::string build_flexray_asc_line(
		const FlexRayFrame& frame,
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

		const int data_len = std::clamp<int>(static_cast<int>(frame.length), 0, 64);

		std::ostringstream oss;
		oss.setf(std::ios::fixed);
		oss << std::setprecision(6);
		oss << t_sec << " FLEXRAY "
			<< frame.channel << ' '
			<< asc_dir_from_flexray_dir(frame.dir) << ' '
			<< frame.frame_id << ' '
			<< static_cast<int>(frame.cycle) << ' '
			<< data_len << ' '
			<< frame.header_crc << ' '
			<< frame.frame_state;

		oss << std::hex << std::uppercase << std::setfill('0');
		for (int i = 0; i < data_len; ++i)
			oss << ' ' << std::setw(2) << static_cast<int>(frame.data[i]);
		oss << "\r\n";
		return oss.str();
	}

	uint32_t timestamp_unit_{};
};

} // namespace

GWLOGGER_REGISTER_ASC_FRAME_WRITER(FlexRayMessageAscWriter, BusType::FLEXRAY)

} // namespace GWLogger::Asc
