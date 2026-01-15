#include "can_message2.h"
#include "../../include/can_object.h"
#include "../../blf/blf_object_header.h"
#include "../../include/message_factory.h"
#include "../registry/reader_registrar.h"

namespace BLF
{

struct BLF_API CanMessage2::Impl
{
	CanFrame2 frame_{};

	uint64_t timestamp_{};
};

CanMessage2::CanMessage2(const CanFrame2& frame_data)
	: impl(std::make_unique<Impl>())
{
	impl->frame_ = frame_data;
}

CanMessage2::~CanMessage2() = default;

BusType CanMessage2::get_bus_type() const
{
	return BusType::CAN2;
}

[[nodiscard]] uint64_t CanMessage2::get_timestamp() const
{
	return impl->timestamp_;
}

void CanMessage2::set_timestamp(uint64_t timestamp)
{
	impl->timestamp_ = timestamp;
}

BLF_API const CanFrame2& CanMessage2::get_frame() const
{
	return impl->frame_;
}

}