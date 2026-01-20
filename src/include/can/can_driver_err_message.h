#ifndef CAN_DRIVER_ERR_MESSAGE_H
#define CAN_DRIVER_ERR_MESSAGE_H
#include "can_object.h"
#include "message_factory.h"

namespace GWLogger
{

class GWLOGGER_API CanDriverErrMessage : public BusMessage
{
public:
	explicit CanDriverErrMessage(const CanDriverErrFrame& frame);
	~CanDriverErrMessage() override;

	[[nodiscard]] BusType get_bus_type() const override;

	[[nodiscard]] uint64_t get_timestamp() const override;

	void set_timestamp(uint64_t timestamp) override;

	[[nodiscard]] const CanDriverErrFrame& get_frame() const;

private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};

template <>
struct MessageType<CanDriverErrFrame> { using type = CanDriverErrMessage; };

}

#endif //CAN_DRIVER_ERR_MESSAGE_H
