#ifndef CAN_MESSAGE_H
#define CAN_MESSAGE_H
#include "../../include/types.h"
#include "../../include/can_object.h"

namespace BLF
{

class BLF_API CanMessage : public BusMessage
{
public:
	explicit CanMessage(const CanFrame& frame_data);
	~CanMessage() override;

	[[nodiscard]] BusType get_bus_type() const override;

	[[nodiscard]] uint64_t get_timestamp() const override;

	void set_timestamp(uint64_t timestamp) override;

	[[nodiscard]] const CanFrame& get_frame() const;
private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};

}

#include "../../include/message_factory.h"
namespace BLF
{
	template <>
	struct MessageType<CanFrame>
	{
		using type = CanMessage;
	};
}

#endif //CAN_MESSAGE_H
