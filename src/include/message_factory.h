#ifndef MESSAGE_FACTORY_H
#define MESSAGE_FACTORY_H

#include "types.h"
#include "can_object.h"
#include <memory>
#include <type_traits>

namespace BLF {

// 配套的销毁函数，也必须从 DLL 导出
BLF_API void destroy_message(BusMessage* message);

// 可选的辅助类，方便用户使用

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
