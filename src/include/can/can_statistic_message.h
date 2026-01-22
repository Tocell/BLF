#ifndef CAN_STATISTIC_MESSAGE_H
#define CAN_STATISTIC_MESSAGE_H
#include "can_object.h"
#include "message_factory.h"

namespace GWLogger
{

class GWLOGGER_API CanStatisticMessage : public BusMessage
{
public:
    explicit CanStatisticMessage(const CanDriverStatisticFrame& frame);
    ~CanStatisticMessage() override;

    [[nodiscard]] BusType get_bus_type() const override;

    [[nodiscard]] uint64_t get_timestamp() const override;

    void set_timestamp(uint64_t timestamp) override;

    [[nodiscard]] const CanDriverStatisticFrame& get_frame() const;
private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

// 工厂模式显示特化
template <>
struct MessageType<CanDriverStatisticFrame> { using type = CanStatisticMessage; };

}

#endif //CAN_STATISTIC_MESSAGE_H
