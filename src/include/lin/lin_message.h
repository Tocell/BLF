#ifndef LIN_MESSAGE_H
#define LIN_MESSAGE_H

#include "lin_object.h"
#include "message_factory.h"

namespace GWLogger
{

class GWLOGGER_API LinMessage : public BusMessage
{
public:
	explicit LinMessage(const LinFrame& frame);
	~LinMessage() override;

	[[nodiscard]] BusType get_bus_type() const override;
	[[nodiscard]] uint64_t get_timestamp() const override;
	void set_timestamp(uint64_t timestamp) override;
	[[nodiscard]] const LinFrame& get_frame() const;

private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};

template <>
struct MessageType<LinFrame> { using type = LinMessage; };

}

#endif //LIN_MESSAGE_H
