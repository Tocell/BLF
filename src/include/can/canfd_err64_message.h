#ifndef CANFD_ERR64_MESSAGE_H
#define CANFD_ERR64_MESSAGE_H
#include "can_object.h"
#include "message_factory.h"

namespace GWLogger
{

class GWLOGGER_API CanFdErr64Message : public BusMessage
{
public:
    explicit CanFdErr64Message(const CanFdErr64Frame& frame);
    ~CanFdErr64Message() override;

    [[nodiscard]] BusType get_bus_type() const override;

    [[nodiscard]] uint64_t get_timestamp() const override;

    void set_timestamp(uint64_t timestamp) override;

    [[nodiscard]] const CanFdErr64Frame& get_frame() const;
private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

// 工厂模式显示特化
template <>
struct MessageType<CanFdErr64Frame> { using type = CanFdErr64Message; };

}

#endif //CANFD_ERR64_MESSAGE_H
