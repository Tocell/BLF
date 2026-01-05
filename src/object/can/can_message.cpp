#include "can_message.h"
#include "../../include/can_object.h"
#include "../../blf/blf_object_header.h"
#include "message_factory.h"

namespace BLF
{
struct BLF_API CanMessage::Impl
{
	ObjectHeaderBase header_base_{};
	ObjectHeader header_{};
	CanFrame frame_{};
};

CanMessage::CanMessage(const CanFrame& frame_data)
	: impl(std::make_unique<Impl>())
{
	impl->frame_ = frame_data;
}

CanMessage::~CanMessage() = default;

BusType CanMessage::get_bus_type() const
{
	return BusType::CAN;
}

[[nodiscard]] uint64_t CanMessage::get_timestamp() const
{
	return impl->header_.object_timestamp;
}

void CanMessage::set_timestamp(uint64_t timestamp)
{
	impl->header_.object_timestamp = timestamp;
}

BLF_API BusMessage* create_message(const CanFrame& frame)
{
	// 使用 new，返回原始指针
	return new CanMessage(frame);
}

BLF_API void BLF::destroy_message(BusMessage* message)
{
	// 在 DLL 内部 delete，保证分配和释放在同一个模块
	delete message;
}

BLF_API const CanFrame& CanMessage::get_frame() const
{
	return impl->frame_;
}

}