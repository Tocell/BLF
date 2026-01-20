#ifndef CAN_ERROR_MESSAGE_H
#define CAN_ERROR_MESSAGE_H
#include "can_object.h"
#include "message_factory.h"

namespace GWLogger
{

class GWLOGGER_API CanErrMessage : public BusMessage
{
public:
    explicit CanErrMessage(const CanErrFrame& frame);
    ~CanErrMessage() override;

    [[nodiscard]] BusType get_bus_type() const override;

    [[nodiscard]] uint64_t get_timestamp() const override;

    void set_timestamp(uint64_t timestamp) override;

    [[nodiscard]] const CanErrFrame& get_frame() const;
private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

// 工厂模式显示特化
template <>
struct MessageType<CanErrFrame> { using type = CanErrMessage; };

}
#endif //CAN_ERROR_MESSAGE_H
