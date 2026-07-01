#include "can/can_error_message.h"
#include "asc_frame_registration.h"
#include "asc_reader_helper.h"

#include <string>

namespace GWLogger::Asc
{

namespace
{

class CanErrorMessageAscReader final : public IAscMessageReader
{
public:
	BusMessagePtr read_line(const std::string& line, uint64_t file_start_posix_us) const override
	{
		auto t = split_ws(line);
		if (t.size() < 3) return nullptr;

		double time_sec = 0.0;
		if (!parse_double_strict(t[0], time_sec)) return nullptr;
		if (time_sec < 0) time_sec = 0;

		int ch = 0;
		if (!parse_int_strict(t[1], ch) || ch < 0 || ch > 0xFFFF) return nullptr;
		if (t[2] != "ErrorFrame") return nullptr;

		CanErrFrame f{};
		f.channel = static_cast<uint16_t>(ch);

		const auto delta_us = static_cast<uint64_t>(time_sec * 1000000.0 + 0.5);
		const uint64_t posix_us = file_start_posix_us + delta_us;

		BusMessagePtr msg = make_message(f);
		if (!msg) return nullptr;
		msg->set_timestamp(posix_us);
		return msg;
	}

	bool match(const std::string& line) const override
	{
		auto t = split_ws(line);
		if (t.size() < 3) return false;

		double time_sec = 0.0;
		if (!parse_double_strict(t[0], time_sec)) return false;

		int ch = 0;
		if (!parse_int_strict(t[1], ch) || ch < 0 || ch > 0xFFFF) return false;

		return t[2] == "ErrorFrame";
	}
};

} // namespace

GWLOGGER_REGISTER_ASC_FRAME_READER(CanErrorMessageAscReader, AscLineKey::CanError)

} // namespace GWLogger::Asc
