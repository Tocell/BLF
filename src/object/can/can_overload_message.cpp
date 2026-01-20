#include "can_overload_message.h"

namespace GWLogger
{

struct GWLOGGER_API CanOverloadMessage::Impl
{
    CanOverloadFrame frame_{};

    uint64_t timestamp_{};
};

CanOverloadMessage::CanOverloadMessage(const CanOverloadFrame& frame_data)
    : impl(std::make_unique<Impl>())
{
    impl->frame_ = frame_data;
}

CanOverloadMessage::~CanOverloadMessage() = default;

BusType CanOverloadMessage::get_bus_type() const
{
    return BusType::CAN_OVERLOAD;
}

[[nodiscard]] uint64_t CanOverloadMessage::get_timestamp() const
{
    return impl->timestamp_;
}

void CanOverloadMessage::set_timestamp(uint64_t timestamp)
{
    impl->timestamp_ = timestamp;
}

GWLOGGER_API const CanOverloadFrame& CanOverloadMessage::get_frame() const
{
    return impl->frame_;
}

}