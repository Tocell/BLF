#ifndef CANFD_MESSAGE_H
#define CANFD_MESSAGE_H
#include <memory>

#include "../../include/bus_message.h"
#include "../../include/can_object.h"
#include "../../include/message_factory.h"

namespace BLF
{

class BLF_API CanFdMessage : public BusMessage
{
public:
	explicit CanFdMessage(const CanFdFrame& frame);
	~CanFdMessage() override;

	[[nodiscard]] BusType get_bus_type() const override;

	[[nodiscard]] uint64_t get_timestamp() const override;

	void set_timestamp(uint64_t timestamp) override;

	[[nodiscard]] const CanFdFrame& get_frame() const;

private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};

template <>
struct MessageType<CanFdFrame> { using type = CanFdMessage; };

}

#endif //CANFD_MESSAGE_H
