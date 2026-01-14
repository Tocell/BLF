#ifndef MESSAGE_FACTORY_H
#define MESSAGE_FACTORY_H

#include "../../include/types.h"
#include "../../include/can_object.h" // 假设 CanFrame 在这里
#include <memory>

namespace BLF {

	// 工厂函数返回一个原始指针
	BLF_API BusMessage* create_message(const CanFrame& frame);

	BLF_API BusMessage* create_message(const CanFrame2& frame);

	// 配套的销毁函数，也必须从 DLL 导出
	BLF_API inline void destroy_message(BusMessage* message)
	{
		delete message;
	}

	// 可选的辅助类，方便用户使用
	struct MessageDeleter {
		void operator()(BusMessage* ptr) const {
			destroy_message(ptr);
		}
	};

	using BusMessagePtr = std::unique_ptr<BusMessage, MessageDeleter>;

} // namespace BLF

#endif //MESSAGE_FACTORY_H
