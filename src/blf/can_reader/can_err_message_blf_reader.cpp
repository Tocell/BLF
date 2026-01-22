#include "can_err_message_blf_reader.h"
#include "blf_object_header.h"
#include "blf_reader_registrar.h"
#include "can_error_message.h"

namespace GWLogger::Blf
{

static ReaderRegistrar<CanErrMessageBlfReader> registrar(BL_OBJ_TYPE_CAN_ERROR);

CanErrMessageBlfReader::CanErrMessageBlfReader() = default;

BusMessagePtr CanErrMessageBlfReader::read(const uint8_t* obj_begin, size_t obj_size)
{
	if (obj_size < sizeof(ObjectHeaderBase) + sizeof(ObjectHeader) + sizeof(CanErrFrame)) {
		return nullptr;
	}

	auto* base = reinterpret_cast<const ObjectHeaderBase*>(obj_begin);
	if (base->signature != BL_OBJ_SIGNATURE) return nullptr;
	if (base->object_type != BL_OBJ_TYPE_CAN_ERROR) return nullptr;

	const auto* frame = reinterpret_cast<const CanErrFrame*>(obj_begin + sizeof(ObjectHeaderBase) + sizeof(ObjectHeader));

	BusMessagePtr msg = make_message(*frame);

	return msg;
}

}
