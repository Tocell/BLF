#include "can_err_ext_message_blf_reader.h"
#include "blf_object_header.h"
#include "blf_reader_registrar.h"
#include "can_err_ext_message.h"

namespace GWLogger::Blf
{

static ReaderRegistrar<CanErrExtMessageBlfReader> registrar(BL_OBJ_TYPE_CAN_ERROR_EXT);

CanErrExtMessageBlfReader::CanErrExtMessageBlfReader() = default;

BusMessagePtr CanErrExtMessageBlfReader::read(const uint8_t* obj_begin, size_t obj_size)
{
	if (obj_size < sizeof(ObjectHeaderBase) + sizeof(ObjectHeader) + sizeof(CanErrExtFrame)) {
		return nullptr;
	}

	auto* base = reinterpret_cast<const ObjectHeaderBase*>(obj_begin);
	if (base->signature != BL_OBJ_SIGNATURE) return nullptr;
	if (base->object_type != BL_OBJ_TYPE_CAN_ERROR_EXT) return nullptr;

	const auto* frame = reinterpret_cast<const CanErrExtFrame*>(obj_begin + sizeof(ObjectHeaderBase) + sizeof(ObjectHeader));

	BusMessagePtr msg = make_message(*frame);

	return msg;
}

}
