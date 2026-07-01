#include "ethernet/ethernet_message.h"
#include "asc_frame_registration.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

namespace GWLogger::Asc
{

namespace
{

class EthernetFrameAscWriter final : public IMessageWriter
{
public:
	bool write(const BusMessage& msg, FileWriter& writer) override
	{
		const auto& ethernet_msg = dynamic_cast<const EthernetMessage&>(msg);
		const auto& frame = ethernet_msg.get_frame();
		auto line = build_ethernet_asc_line(
			frame,
			ethernet_msg.get_timestamp(),
			writer.get_file_start_time(),
			false);
		writer.append(reinterpret_cast<const uint8_t*>(line.data()), line.size());
		return true;
	}

	[[nodiscard]] size_t estimate_write_size(const BusMessage& msg) const override
	{
		const auto* ethernet_msg = dynamic_cast<const EthernetMessage*>(&msg);
		if (!ethernet_msg)
			return 400;

		const auto payload_length = std::min<size_t>(
			ethernet_msg->get_frame().payload_length,
			ETHERNET_MAX_PAYLOAD_LENGTH);
		return 128 + payload_length * 3;
	}

	void set_timestamp_unit(TimeStampUnit unit) override
	{
		timestamp_unit_ = static_cast<uint32_t>(unit);
	}

private:
	static inline const char* asc_dir_from_eth_dir(uint16_t dir)
	{
		if (dir == 1) return "Tx";
		if (dir == 2) return "TxRq";
		return "Rx";
	}

	static inline void append_mac(std::ostringstream& oss, const uint8_t address[6])
	{
		oss << std::hex << std::uppercase << std::setfill('0');
		for (int i = 0; i < 6; ++i)
		{
			if (i != 0) oss << ':';
			oss << std::setw(2) << static_cast<int>(address[i]);
		}
		oss << std::dec;
	}

	static inline std::string build_ethernet_asc_line(
		const EthernetFrame& frame,
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

		const auto payload_length = std::min<uint16_t>(
			frame.payload_length,
			ETHERNET_MAX_PAYLOAD_LENGTH);

		std::ostringstream oss;
		oss.setf(std::ios::fixed);
		oss << std::setprecision(6);
		oss << t_sec << " ETHERNET "
			<< frame.channel << ' '
			<< asc_dir_from_eth_dir(frame.dir) << ' ';
		append_mac(oss, frame.source_address);
		oss << ' ';
		append_mac(oss, frame.destination_address);
		oss << ' '
			<< std::hex << std::uppercase
			<< frame.type << ' '
			<< frame.tpid << ' '
			<< frame.tci
			<< std::dec << ' '
			<< payload_length;

		oss << std::hex << std::uppercase << std::setfill('0');
		for (uint16_t i = 0; i < payload_length; ++i)
			oss << ' ' << std::setw(2) << static_cast<int>(frame.payload[i]);
		oss << "\r\n";
		return oss.str();
	}

	uint32_t timestamp_unit_{};
};

} // namespace

GWLOGGER_REGISTER_ASC_FRAME_WRITER(EthernetFrameAscWriter, BusType::ETHERNET)

} // namespace GWLogger::Asc
