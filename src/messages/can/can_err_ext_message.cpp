#include "can_err_ext_message.h"

namespace GWLogger
{

struct GWLOGGER_API CanErrExtMessage::Impl
{
    CanErrExtFrame frame_{};

    uint64_t timestamp_{};
};

CanErrExtMessage::CanErrExtMessage(const CanErrExtFrame& frame_data)
    : impl(std::make_unique<Impl>())
{
    impl->frame_ = frame_data;
}

CanErrExtMessage::~CanErrExtMessage() = default;

BusType CanErrExtMessage::get_bus_type() const
{
    return BusType::CAN_ERR_EXT;
}

[[nodiscard]] uint64_t CanErrExtMessage::get_timestamp() const
{
    return impl->timestamp_;
}

void CanErrExtMessage::set_timestamp(uint64_t timestamp)
{
    impl->timestamp_ = timestamp;
}

GWLOGGER_API const CanErrExtFrame& CanErrExtMessage::get_frame() const
{
    return impl->frame_;
}

}