#include "can_overload_message_blf_reader.h"
#include "blf_object_header.h"
#include "blf_reader_registrar.h"
#include "can_overload_message.h"

namespace GWLogger::Blf
{

static ReaderRegistrar<CanOverloadMessageBlfReader> registrar(BL_OBJ_TYPE_CAN_OVERLOAD);

CanOverloadMessageBlfReader::CanOverloadMessageBlfReader() = default;

BusMessagePtr CanOverloadMessageBlfReader::read(const uint8_t* obj_begin, size_t obj_size)
{
	if (obj_size < sizeof(ObjectHeaderBase) + sizeof(ObjectHeader) + sizeof(CanOverloadFrame))
	{
		return nullptr;
	}

	auto* base = reinterpret_cast<const ObjectHeaderBase*>(obj_begin);
	if (base->signature != BL_OBJ_SIGNATURE) return nullptr;
	if (base->object_type != BL_OBJ_TYPE_CAN_OVERLOAD) return nullptr;

	const auto* frame = reinterpret_cast<const CanOverloadFrame*>(obj_begin + sizeof(ObjectHeaderBase) + sizeof(ObjectHeader));

	BusMessagePtr msg = make_message(*frame);

	return msg;
}

}
