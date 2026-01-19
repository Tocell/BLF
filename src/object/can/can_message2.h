#ifndef CAN_MESSAGE2_H
#define CAN_MESSAGE2_H
#include "../../include/bus_message.h"
#include "../../include/can_object.h"
#include "../../include/message_factory.h"

namespace BLF
{

class BLF_API CanMessage2 : public BusMessage
{
public:
	explicit CanMessage2(const CanFrame2& frame_data);
	~CanMessage2() override;

	[[nodiscard]] BusType get_bus_type() const override;

	[[nodiscard]] uint64_t get_timestamp() const override;

	void set_timestamp(uint64_t timestamp) override;

	[[nodiscard]] const CanFrame2& get_frame() const;
private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};

// 工厂模式显示特化
template <>
struct MessageType<CanFrame2> { using type = CanMessage2; };

}

#endif //CAN_MESSAGE2_H
