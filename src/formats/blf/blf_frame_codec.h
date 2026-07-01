#ifndef BLF_FRAME_CODEC_H
#define BLF_FRAME_CODEC_H

#include "blf_object_header.h"
#include "imessage_reader.h"
#include "imessage_writer.h"

#include <cstring>
#include <type_traits>

namespace GWLogger::Blf
{

inline uint64_t encode_blf_timestamp(uint64_t message_time_ns,
                                     uint64_t file_start_time_us,
                                     uint32_t time_flags)
{
	const uint64_t file_start_time_ns = file_start_time_us * 1000ULL;
	const uint64_t delta_ns = message_time_ns > file_start_time_ns
		? message_time_ns - file_start_time_ns
		: 0ULL;

	return time_flags == BL_OBJ_FLAG_TIME_TEN_MICS
		? delta_ns / 10000ULL
		: delta_ns;
}

template<typename T>
inline void append_pod(FileWriter& writer, const T& value)
{
	static_assert(std::is_trivially_copyable_v<T>, "BLF serialization requires POD-like frame types");
	writer.append(reinterpret_cast<const uint8_t*>(&value), sizeof(T));
}

template<typename MessageT, typename FrameT, uint32_t ObjectType>
class BlfFrameWriter : public IMessageWriter
{
public:
	BlfFrameWriter()
	{
		header_base_.signature = BL_OBJ_SIGNATURE;
		header_base_.header_size = sizeof(ObjectHeaderBase) + sizeof(ObjectHeader);
		header_base_.object_size = sizeof(ObjectHeaderBase) + sizeof(ObjectHeader) + sizeof(FrameT);
		header_base_.object_type = ObjectType;
		header_base_.header_version = 1;

		header_.client_index = 1;
		header_.object_timestamp = 0;
		header_.object_version = 0;
		header_.time_flags = BL_OBJ_FLAG_TIME_ONE_NANS;
	}

	bool write(const BusMessage& msg, FileWriter& writer) override
	{
		const auto* typed_msg = dynamic_cast<const MessageT*>(&msg);
		if (!typed_msg)
		{
			return false;
		}

		header_.object_timestamp = encode_blf_timestamp(
			typed_msg->get_timestamp(),
			writer.get_file_start_time(),
			header_.time_flags);

		append_pod(writer, header_base_);
		append_pod(writer, header_);
		append_pod(writer, typed_msg->get_frame());
		return true;
	}

	void set_timestamp_unit(TimeStampUnit unit) override
	{
		switch (unit)
		{
		case TimeStampUnit::BLF_TIME_TEN_MICS:
			header_.time_flags = BL_OBJ_FLAG_TIME_TEN_MICS;
			break;
		case TimeStampUnit::BLF_TIME_ONE_NANS:
		default:
			header_.time_flags = BL_OBJ_FLAG_TIME_ONE_NANS;
			break;
		}
	}

private:
	ObjectHeaderBase header_base_{};
	ObjectHeader header_{};
};

template<typename FrameT, uint32_t ObjectType>
class BlfFrameReader : public IMessageReader
{
public:
	BusMessagePtr read(const uint8_t* obj_begin, size_t obj_size) override
	{
		constexpr size_t kFrameOffset = sizeof(ObjectHeaderBase) + sizeof(ObjectHeader);
		constexpr size_t kMinSize = kFrameOffset + sizeof(FrameT);

		if (!obj_begin || obj_size < kMinSize)
		{
			return nullptr;
		}

		ObjectHeaderBase base{};
		std::memcpy(&base, obj_begin, sizeof(base));
		if (base.signature != BL_OBJ_SIGNATURE || base.object_type != ObjectType)
		{
			return nullptr;
		}

		FrameT frame{};
		std::memcpy(&frame, obj_begin + kFrameOffset, sizeof(frame));
		return make_message(frame);
	}
};

}

#endif // BLF_FRAME_CODEC_H
