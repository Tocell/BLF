#include "can_statistic_message.h"

namespace GWLogger
{

struct GWLOGGER_API CanStatisticMessage::Impl
{
    CanDriverStatisticFrame frame_{};

    uint64_t timestamp_{};
};

CanStatisticMessage::CanStatisticMessage(const CanDriverStatisticFrame& frame_data)
    : impl(std::make_unique<Impl>())
{
    impl->frame_ = frame_data;
}

CanStatisticMessage::~CanStatisticMessage() = default;

BusType CanStatisticMessage::get_bus_type() const
{
    return BusType::CAN_STATISTIC;
}

[[nodiscard]] uint64_t CanStatisticMessage::get_timestamp() const
{
    return impl->timestamp_;
}

void CanStatisticMessage::set_timestamp(uint64_t timestamp)
{
    impl->timestamp_ = timestamp;
}

GWLOGGER_API const CanDriverStatisticFrame& CanStatisticMessage::get_frame() const
{
    return impl->frame_;
}

}