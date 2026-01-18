#ifndef CAN_MESSAGE_BLF_READER_H
#define CAN_MESSAGE_BLF_READER_H
#include "../../api/imessage_reader.h"
#include "../blf_object_header.h"
#include "../../io/file_reader.h"

namespace BLF{

class CanMessageBlfReader : public IMessageReader
{
public:
	CanMessageBlfReader();
	~CanMessageBlfReader() override = default;
	BusMessagePtr read(const uint8_t* obj_begin, size_t obj_size) override;

};

};

#endif //CAN_MESSAGE_BLF_READER_H
