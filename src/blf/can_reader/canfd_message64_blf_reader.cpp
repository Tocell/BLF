#include "canfd_message64_blf_reader.h"
#include "blf_object_header.h"
#include "blf_reader_registrar.h"
#include "canfd_message64.h"

namespace GWLogger::Blf
{

static ReaderRegistrar<CanFdMessage64BlfReader> registrar(BL_OBJ_TYPE_CAN_FD_MESSAGE_64);

CanFdMessage64BlfReader::CanFdMessage64BlfReader() = default;

BusMessagePtr CanFdMessage64BlfReader::read(const uint8_t* obj_begin, size_t obj_size)
{
	if (obj_size < sizeof(ObjectHeaderBase) + sizeof(ObjectHeader) + sizeof(CanFdFrame64)) {
		return nullptr;
	}

	auto* base = reinterpret_cast<const ObjectHeaderBase*>(obj_begin);
	if (base->signature != BL_OBJ_SIGNATURE) return nullptr;
	if (base->object_type != BL_OBJ_TYPE_CAN_FD_MESSAGE_64) return nullptr;

	const auto* frame = reinterpret_cast<const CanFdFrame64*>(obj_begin + sizeof(ObjectHeaderBase) + sizeof(ObjectHeader));

	BusMessagePtr msg = make_message(*frame);

	return msg;
}

}
