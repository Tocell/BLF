#include "ethernet/ethernet_message.h"

namespace GWLogger
{

struct GWLOGGER_API EthernetMessage::Impl
{
	EthernetFrame frame_{};
	uint64_t timestamp_{};
};

EthernetMessage::EthernetMessage(const EthernetFrame& frame)
	: impl(std::make_unique<Impl>())
{
	impl->frame_ = frame;
}

EthernetMessage::~EthernetMessage() = default;

BusType EthernetMessage::get_bus_type() const
{
	return BusType::ETHERNET;
}

uint64_t EthernetMessage::get_timestamp() const
{
	return impl->timestamp_;
}

void EthernetMessage::set_timestamp(uint64_t timestamp)
{
	impl->timestamp_ = timestamp;
}

const EthernetFrame& EthernetMessage::get_frame() const
{
	return impl->frame_;
}

}
