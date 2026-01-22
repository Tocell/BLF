#include "can_driver_err_ext_message.h"

namespace GWLogger
{

struct GWLOGGER_API CanDriverErrExtMessage::Impl
{
	CanDriverErrExtFrame frame_{};

	uint64_t timestamp_{};
};

CanDriverErrExtMessage::CanDriverErrExtMessage(const CanDriverErrExtFrame& frame_data)
	: impl(std::make_unique<Impl>())
{
	impl->frame_ = frame_data;
}

CanDriverErrExtMessage::~CanDriverErrExtMessage() = default;

BusType CanDriverErrExtMessage::get_bus_type() const
{
	return BusType::CAN_DRIVER_ERR_EXT;
}

[[nodiscard]] uint64_t CanDriverErrExtMessage::get_timestamp() const
{
	return impl->timestamp_;
}

void CanDriverErrExtMessage::set_timestamp(uint64_t timestamp)
{
	impl->timestamp_ = timestamp;
}

GWLOGGER_API const CanDriverErrExtFrame& CanDriverErrExtMessage::get_frame() const
{
	return impl->frame_;
}

}