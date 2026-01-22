#include "can_driver_err_message.h"

namespace GWLogger
{

struct GWLOGGER_API CanDriverErrMessage::Impl
{
	CanDriverErrFrame frame_{};

	uint64_t timestamp_{};
};

CanDriverErrMessage::CanDriverErrMessage(const CanDriverErrFrame& frame_data)
	: impl(std::make_unique<Impl>())
{
	impl->frame_ = frame_data;
}

CanDriverErrMessage::~CanDriverErrMessage() = default;

BusType CanDriverErrMessage::get_bus_type() const
{
	return BusType::CAN_DRIVER_ERR;
}

[[nodiscard]] uint64_t CanDriverErrMessage::get_timestamp() const
{
	return impl->timestamp_;
}

void CanDriverErrMessage::set_timestamp(uint64_t timestamp)
{
	impl->timestamp_ = timestamp;
}

GWLOGGER_API const CanDriverErrFrame& CanDriverErrMessage::get_frame() const
{
	return impl->frame_;
}

}