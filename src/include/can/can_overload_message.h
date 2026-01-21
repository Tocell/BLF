#ifndef CAN_OVERLOAD_MESSAGE_H
#define CAN_OVERLOAD_MESSAGE_H
#include "can_object.h"
#include "message_factory.h"

namespace GWLogger
{

class GWLOGGER_API CanOverloadMessage : public BusMessage
{
public:
    explicit CanOverloadMessage(const CanOverloadFrame& frame);
    ~CanOverloadMessage() override;

    [[nodiscard]] BusType get_bus_type() const override;

    [[nodiscard]] uint64_t get_timestamp() const override;

    void set_timestamp(uint64_t timestamp) override;

    [[nodiscard]] const CanOverloadFrame& get_frame() const;
private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

// 工厂模式显示特化
template <>
struct MessageType<CanOverloadFrame> { using type = CanOverloadMessage; };

}
#endif //CAN_OVERLOAD_MESSAGE_H
