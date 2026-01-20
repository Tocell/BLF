#include "can_error_message.h"

namespace GWLogger
{

struct GWLOGGER_API CanErrMessage::Impl
{
    CanErrFrame frame_{};

    uint64_t timestamp_{};
};

CanErrMessage::CanErrMessage(const CanErrFrame& frame_data)
    : impl(std::make_unique<Impl>())
{
    impl->frame_ = frame_data;
}

CanErrMessage::~CanErrMessage() = default;

BusType CanErrMessage::get_bus_type() const
{
    return BusType::CAN_ERR;
}

[[nodiscard]] uint64_t CanErrMessage::get_timestamp() const
{
    return impl->timestamp_;
}

void CanErrMessage::set_timestamp(uint64_t timestamp)
{
    impl->timestamp_ = timestamp;
}

GWLOGGER_API const CanErrFrame& CanErrMessage::get_frame() const
{
    return impl->frame_;
}

}