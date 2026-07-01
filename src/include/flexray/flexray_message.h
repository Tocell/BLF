#ifndef FLEXRAY_MESSAGE_H
#define FLEXRAY_MESSAGE_H

#include "flexray_object.h"
#include "message_factory.h"

namespace GWLogger
{

class GWLOGGER_API FlexRayMessage : public BusMessage
{
public:
	explicit FlexRayMessage(const FlexRayFrame& frame);
	~FlexRayMessage() override;

	[[nodiscard]] BusType get_bus_type() const override;
	[[nodiscard]] uint64_t get_timestamp() const override;
	void set_timestamp(uint64_t timestamp) override;
	[[nodiscard]] const FlexRayFrame& get_frame() const;

private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};

template <>
struct MessageType<FlexRayFrame> { using type = FlexRayMessage; };

}

#endif //FLEXRAY_MESSAGE_H
