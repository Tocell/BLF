#include "flexray/flexray_message.h"

namespace GWLogger
{

struct GWLOGGER_API FlexRayMessage::Impl
{
	FlexRayFrame frame_{};
	uint64_t timestamp_{};
};

FlexRayMessage::FlexRayMessage(const FlexRayFrame& frame)
	: impl(std::make_unique<Impl>())
{
	impl->frame_ = frame;
}

FlexRayMessage::~FlexRayMessage() = default;

BusType FlexRayMessage::get_bus_type() const
{
	return BusType::FLEXRAY;
}

uint64_t FlexRayMessage::get_timestamp() const
{
	return impl->timestamp_;
}

void FlexRayMessage::set_timestamp(uint64_t timestamp)
{
	impl->timestamp_ = timestamp;
}

const FlexRayFrame& FlexRayMessage::get_frame() const
{
	return impl->frame_;
}

}
