#include "can_driver_sync_message.h"

namespace GWLogger
{

struct GWLOGGER_API CanDriverSyncMessage::Impl
{
    CanDriverSyncFrame frame_{};

    uint64_t timestamp_{};
};

CanDriverSyncMessage::CanDriverSyncMessage(const CanDriverSyncFrame& frame_data)
    : impl(std::make_unique<Impl>())
{
    impl->frame_ = frame_data;
}

CanDriverSyncMessage::~CanDriverSyncMessage() = default;

BusType CanDriverSyncMessage::get_bus_type() const
{
    return BusType::CAN_DRIVER_SYNC;
}

[[nodiscard]] uint64_t CanDriverSyncMessage::get_timestamp() const
{
    return impl->timestamp_;
}

void CanDriverSyncMessage::set_timestamp(uint64_t timestamp)
{
    impl->timestamp_ = timestamp;
}

GWLOGGER_API const CanDriverSyncFrame& CanDriverSyncMessage::get_frame() const
{
    return impl->frame_;
}

}
