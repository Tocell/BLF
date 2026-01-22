#include "canfd_message_blf_reader.h"
#include "blf_object_header.h"
#include "blf_reader_registrar.h"
#include "canfd_message.h"

namespace GWLogger::Blf
{

static ReaderRegistrar<CanFdMessageBlfReader> registrar(BL_OBJ_TYPE_CAN_FD_MESSAGE);

CanFdMessageBlfReader::CanFdMessageBlfReader() = default;

BusMessagePtr CanFdMessageBlfReader::read(const uint8_t* obj_begin, size_t obj_size)
{
	if (obj_size < sizeof(ObjectHeaderBase) + sizeof(ObjectHeader) + sizeof(CanFdFrame)) {
		return nullptr;
	}

	auto* base = reinterpret_cast<const ObjectHeaderBase*>(obj_begin);
	if (base->signature != BL_OBJ_SIGNATURE) return nullptr;
	if (base->object_type != BL_OBJ_TYPE_CAN_FD_MESSAGE) return nullptr;

	const auto* frame = reinterpret_cast<const CanFdFrame*>(obj_begin + sizeof(ObjectHeaderBase) + sizeof(ObjectHeader));

	BusMessagePtr msg = make_message(*frame);

	return msg;
}

}
