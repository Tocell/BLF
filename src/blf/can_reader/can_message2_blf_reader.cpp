#include "can_message2_blf_reader.h"
#include "blf_object_header.h"
#include "blf_reader_registrar.h"
#include "can_message2.h"

namespace GWLogger::Blf
{

static ReaderRegistrar<CanMessage2BlfReader> registrar(BL_OBJ_TYPE_CAN_MESSAGE2);

CanMessage2BlfReader::CanMessage2BlfReader() = default;

BusMessagePtr CanMessage2BlfReader::read(const uint8_t* obj_begin, size_t obj_size)
{
	if (obj_size < sizeof(ObjectHeaderBase) + sizeof(ObjectHeader) + sizeof(CanFrame2)) {
		return nullptr;
	}

	auto* base = reinterpret_cast<const ObjectHeaderBase*>(obj_begin);
	if (base->signature != BL_OBJ_SIGNATURE) return nullptr;
	if (base->object_type != BL_OBJ_TYPE_CAN_MESSAGE2) return nullptr;

	const auto* frame = reinterpret_cast<const CanFrame2*>(obj_begin + sizeof(ObjectHeaderBase) + sizeof(ObjectHeader));

	BusMessagePtr msg = make_message(*frame);

	return msg;
}

}