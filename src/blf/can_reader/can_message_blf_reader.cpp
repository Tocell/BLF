#include "can_message_blf_reader.h"

#include "../../registry/reader_registrar.h"
#include "../../registry/reader_registry.h"
#include "../../object/can/can_message.h"

namespace BLF
{

static ReaderRegistrar<CanMessageBlfReader> registrar(BL_OBJ_TYPE_CAN_MESSAGE);

CanMessageBlfReader::CanMessageBlfReader() = default;

BusMessagePtr CanMessageBlfReader::read(const uint8_t* obj_begin, size_t obj_size)
{
	if (obj_size < sizeof(ObjectHeaderBase) + sizeof(ObjectHeader) + sizeof(CanFrame)) {
		return nullptr;
	}

	auto* base = reinterpret_cast<const ObjectHeaderBase*>(obj_begin);
	if (base->signature != BL_OBJ_SIGNATURE) return nullptr;
	if (base->object_type != BL_OBJ_TYPE_CAN_MESSAGE) return nullptr;

	const auto* hdr = reinterpret_cast<const ObjectHeader*>(obj_begin + sizeof(ObjectHeaderBase));
	const auto* frame = reinterpret_cast<const CanFrame*>(obj_begin + sizeof(ObjectHeaderBase) + sizeof(ObjectHeader));

	// 1) 先构造 CanMessage
	BusMessagePtr msg = make_message(*frame); // MessageType<CanFrame>::type = CanMessage

	// // 2) 还原 timestamp（你的 writer 写入的是相对 start_time 的 delta）
	// //    writer里：
	// //      ONE_NANS: object_timestamp = msg_ts - file_start_time_us*1000
	// //      TEN_MICS: object_timestamp = (msg_ts - file_start_time_us*1000)/10000
	// uint64_t abs_ns = 0;
	// if (hdr->time_flags == BL_OBJ_FLAG_TIME_ONE_NANS) {
	// 	abs_ns = file_start_time_us * 1000ULL + hdr->object_timestamp;
	// } else if (hdr->time_flags == BL_OBJ_FLAG_TIME_TEN_MICS) {
	// 	abs_ns = file_start_time_us * 1000ULL + hdr->object_timestamp * 10000ULL;
	// } else {
	// 	// 其他单位你没用到的话可以先按 1ns 处理或返回 nullptr
	// 	abs_ns = file_start_time_us * 1000ULL + hdr->object_timestamp;
	// }

	// msg->set_timestamp(abs_ns);
	return msg;
}

}

