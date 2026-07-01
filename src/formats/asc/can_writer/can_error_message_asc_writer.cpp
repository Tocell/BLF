#include "can/can_error_message.h"
#include "asc_frame_registration.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

namespace GWLogger::Asc
{

namespace
{

static inline std::string build_can_error_asc_line(
	const CanErrFrame& frame,
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

	std::ostringstream oss;
	oss.setf(std::ios::fixed);
	oss << std::setprecision(6);
	oss << t_sec << ' ' << frame.channel << " ErrorFrame\r\n";
	return oss.str();
}

class CanErrorMessageAscWriter final : public IMessageWriter
{
public:
	void set_timestamp_unit(TimeStampUnit unit) override
	{
		timestamp_unit_ = static_cast<uint32_t>(unit);
	}

	bool write(const BusMessage& msg, FileWriter& writer) override
	{
		const auto& can_msg = dynamic_cast<const CanErrMessage&>(msg);
		const auto& frame = can_msg.get_frame();

		const auto line = build_can_error_asc_line(
			frame,
			can_msg.get_timestamp(),
			writer.get_file_start_time(),
			false);

		writer.append(reinterpret_cast<const uint8_t*>(line.data()), line.size());
		return true;
	}

private:
	uint32_t timestamp_unit_{};
};

} // namespace

GWLOGGER_REGISTER_ASC_FRAME_WRITER(CanErrorMessageAscWriter, BusType::CAN_ERR)

} // namespace GWLogger::Asc
