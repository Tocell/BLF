#include "canfd_message64.h"

namespace GWLogger
{

struct GWLOGGER_API CanFdMessage64::Impl
{
	CanFdFrame64 frame_{};

	uint64_t timestamp_{};
};

CanFdMessage64::CanFdMessage64(const CanFdFrame64& frame64)
	: impl(std::make_unique<Impl>())
{
	impl->frame_ = frame64;
}

CanFdMessage64::~CanFdMessage64() = default;

BusType CanFdMessage64::get_bus_type() const
{
	return BusType::CAN_FD64;
}

[[nodiscard]] uint64_t CanFdMessage64::get_timestamp() const
{
	return impl->timestamp_;
}

void CanFdMessage64::set_timestamp(uint64_t timestamp)
{
	impl->timestamp_ = timestamp;
}

GWLOGGER_API const CanFdFrame64& CanFdMessage64::get_frame() const
{
	return impl->frame_;
}

}