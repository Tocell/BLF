#include "canfd_err64_message.h"

namespace GWLogger
{
struct GWLOGGER_API CanFdErr64Message::Impl
{
    CanFdErr64Frame frame_{};

    uint64_t timestamp_{};
};

CanFdErr64Message::CanFdErr64Message(const CanFdErr64Frame& frame_data)
    : impl(std::make_unique<Impl>())
{
    impl->frame_ = frame_data;
}

CanFdErr64Message::~CanFdErr64Message() = default;

BusType CanFdErr64Message::get_bus_type() const
{
    return BusType::CANFD_ERR64;
}

[[nodiscard]] uint64_t CanFdErr64Message::get_timestamp() const
{
    return impl->timestamp_;
}

void CanFdErr64Message::set_timestamp(uint64_t timestamp)
{
    impl->timestamp_ = timestamp;
}

GWLOGGER_API const CanFdErr64Frame& CanFdErr64Message::get_frame() const
{
    return impl->frame_;
}
}