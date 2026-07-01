#include "lin/lin_message.h"

namespace GWLogger
{

struct GWLOGGER_API LinMessage::Impl
{
	LinFrame frame_{};
	uint64_t timestamp_{};
};

LinMessage::LinMessage(const LinFrame& frame)
	: impl(std::make_unique<Impl>())
{
	impl->frame_ = frame;
}

LinMessage::~LinMessage() = default;

BusType LinMessage::get_bus_type() const
{
	return BusType::LIN;
}

uint64_t LinMessage::get_timestamp() const
{
	return impl->timestamp_;
}

void LinMessage::set_timestamp(uint64_t timestamp)
{
	impl->timestamp_ = timestamp;
}

const LinFrame& LinMessage::get_frame() const
{
	return impl->frame_;
}

}
