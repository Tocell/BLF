#ifndef CAN_SETTING_CHANGE_MESSAGE_H
#define CAN_SETTING_CHANGE_MESSAGE_H
#include "can_object.h"
#include "message_factory.h"

namespace GWLogger
{

class GWLOGGER_API CanSettigChangedMessage : public BusMessage
{
public:
    explicit CanSettigChangedMessage(const CanSettingChangedFrame& frame);
    ~CanSettigChangedMessage() override;

    [[nodiscard]] BusType get_bus_type() const override;

    [[nodiscard]] uint64_t get_timestamp() const override;

    void set_timestamp(uint64_t timestamp) override;

    [[nodiscard]] const CanSettingChangedFrame& get_frame() const;
private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

// 工厂模式显示特化
template <>
struct MessageType<CanSettingChangedFrame> { using type = CanSettigChangedMessage; };

}

#endif //CAN_SETTING_CHANGE_MESSAGE_H
