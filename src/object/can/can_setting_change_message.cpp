#include "can_setting_change_message.h"

namespace GWLogger
{
struct GWLOGGER_API CanSettigChangedMessage::Impl
{
    CanSettingChangedFrame frame_{};

    uint64_t timestamp_{};
};

CanSettigChangedMessage::CanSettigChangedMessage(const CanSettingChangedFrame& frame_data)
    : impl(std::make_unique<Impl>())
{
    impl->frame_ = frame_data;
}

CanSettigChangedMessage::~CanSettigChangedMessage() = default;

BusType CanSettigChangedMessage::get_bus_type() const
{
    return BusType::CAN_SETTING_CHANGE;
}

[[nodiscard]] uint64_t CanSettigChangedMessage::get_timestamp() const
{
    return impl->timestamp_;
}

void CanSettigChangedMessage::set_timestamp(uint64_t timestamp)
{
    impl->timestamp_ = timestamp;
}

GWLOGGER_API const CanSettingChangedFrame& CanSettigChangedMessage::get_frame() const
{
    return impl->frame_;
}
}