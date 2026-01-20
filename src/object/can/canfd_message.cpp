#include "canfd_message.h"

namespace BLF
{

struct BLF_API CanFdMessage::Impl
{
	CanFdFrame frame_{};
	uint64_t timestamp_{};
};

CanFdMessage::CanFdMessage(const CanFdFrame& frame_data)
	: impl(std::make_unique<Impl>())
{
	impl->frame_ = frame_data;
}

CanFdMessage::~CanFdMessage() = default;

BusType CanFdMessage::get_bus_type() const
{
	return BusType::CAN_FD;
}

[[nodiscard]] uint64_t CanFdMessage::get_timestamp() const
{
	return impl->timestamp_;
}

void CanFdMessage::set_timestamp(uint64_t timestamp)
{
	impl->timestamp_ = timestamp;
}

BLF_API const CanFdFrame& CanFdMessage::get_frame() const
{
	return impl->frame_;
}

}