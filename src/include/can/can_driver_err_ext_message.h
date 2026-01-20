#ifndef CAN_DRIVER_ERR_EXT_MESSAGE_H
#define CAN_DRIVER_ERR_EXT_MESSAGE_H
#include "can_object.h"
#include "message_factory.h"

namespace GWLogger
{

class GWLOGGER_API CanDriverErrExtMessage : public BusMessage
{
public:
	explicit CanDriverErrExtMessage(const CanDriverErrExtFrame& frame);
	~CanDriverErrExtMessage() override;

	[[nodiscard]] BusType get_bus_type() const override;

	[[nodiscard]] uint64_t get_timestamp() const override;

	void set_timestamp(uint64_t timestamp) override;

	[[nodiscard]] const CanDriverErrExtFrame& get_frame() const;

private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};

template <>
struct MessageType<CanDriverErrExtFrame> { using type = CanDriverErrExtMessage; };

}

#endif //CAN_DRIVER_ERR_EXT_MESSAGE_H
