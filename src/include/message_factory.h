#ifndef MESSAGE_FACTORY_H
#define MESSAGE_FACTORY_H

#include "bus_message.h"
#include <memory>
#include <type_traits>

namespace GWLogger {

GWLOGGER_API void destroy_message(BusMessage* message);

struct MessageDeleter {
	void operator()(BusMessage* ptr) const noexcept
	{
		destroy_message(ptr);
	}
};


using BusMessagePtr = std::unique_ptr<BusMessage, MessageDeleter>;

template <typename FrameT>
struct MessageType;

template <typename FrameT>
BusMessage* create_message(const FrameT& frame)
{
	using MsgT = typename MessageType<FrameT>::type;
	static_assert(std::is_base_of_v<BusMessage, MsgT>, "MessageType<FrameT>::type must derive from BusMessage");
	return new MsgT(frame);
}

template <typename FrameT>
BusMessagePtr make_message(const FrameT& frame)
{
	return BusMessagePtr(create_message(frame));
}

} // namespace BLF

#endif //MESSAGE_FACTORY_H
