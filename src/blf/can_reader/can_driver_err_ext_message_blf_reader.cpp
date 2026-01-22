#include "can_driver_err_ext_message_blf_reader.h"
#include "blf_object_header.h"
#include "blf_reader_registrar.h"
#include "can_driver_err_ext_message.h"

namespace GWLogger::Blf
{

static ReaderRegistrar<CanDriver_errExtMessageBlfReader> registrar(BL_OBJ_TYPE_CAN_DRIVER_ERROR_EXT);

CanDriver_errExtMessageBlfReader::CanDriver_errExtMessageBlfReader() = default;

BusMessagePtr CanDriver_errExtMessageBlfReader::read(const uint8_t* obj_begin, size_t obj_size)
{
	if (obj_size < sizeof(ObjectHeaderBase) + sizeof(ObjectHeader) + sizeof(CanDriverErrExtFrame)) {
		return nullptr;
	}

	auto* base = reinterpret_cast<const ObjectHeaderBase*>(obj_begin);
	if (base->signature != BL_OBJ_SIGNATURE) return nullptr;
	if (base->object_type != BL_OBJ_TYPE_CAN_DRIVER_ERROR_EXT) return nullptr;

	const auto* frame = reinterpret_cast<const CanDriverErrExtFrame*>(obj_begin + sizeof(ObjectHeaderBase) + sizeof(ObjectHeader));

	BusMessagePtr msg = make_message(*frame);

	return msg;
}

}

