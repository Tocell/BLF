#include "can_message.h"
#include "can_object.h"
#include "blf_object_header.h"

namespace GWLogger
{

struct GWLOGGER_API CanMessage::Impl
{
	CanFrame frame_{};

	uint64_t timestamp_{};
};

CanMessage::CanMessage(const CanFrame& frame_data)
	: impl(std::make_unique<Impl>())
{
	impl->frame_ = frame_data;
}

CanMessage::~CanMessage() = default;

BusType CanMessage::get_bus_type() const
{
	return BusType::CAN;
}

[[nodiscard]] uint64_t CanMessage::get_timestamp() const
{
	return impl->timestamp_;
}

void CanMessage::set_timestamp(uint64_t timestamp)
{
	impl->timestamp_ = timestamp;
}

GWLOGGER_API const CanFrame& CanMessage::get_frame() const
{
	return impl->frame_;
}

}