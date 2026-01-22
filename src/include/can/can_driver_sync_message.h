#ifndef CAN_DRIVER_SYNC_MESSAGE_H
#define CAN_DRIVER_SYNC_MESSAGE_H

#include "can_object.h"
#include "message_factory.h"

namespace GWLogger
{

    class GWLOGGER_API CanDriverSyncMessage : public BusMessage
    {
    public:
        explicit CanDriverSyncMessage(const CanDriverSyncFrame& frame);
        ~CanDriverSyncMessage() override;

        [[nodiscard]] BusType get_bus_type() const override;

        [[nodiscard]] uint64_t get_timestamp() const override;

        void set_timestamp(uint64_t timestamp) override;

        [[nodiscard]] const CanDriverSyncFrame& get_frame() const;
    private:
        struct Impl;
        std::unique_ptr<Impl> impl;
    };

    // 工厂模式显示特化
    template <>
    struct MessageType<CanDriverSyncFrame> { using type = CanDriverSyncMessage; };

}

#endif //CAN_DRIVER_SYNC_MESSAGE_H
