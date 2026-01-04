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
	~CanMessage();

	[[nodiscard]] BusType get_bus_type() const override;


private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};

}

#endif //CAN_MESSAGE_H
