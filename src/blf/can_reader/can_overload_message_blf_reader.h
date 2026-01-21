#ifndef CAN_OVERLOAD_MESSAGE_BLF_READER_H
#define CAN_OVERLOAD_MESSAGE_BLF_READER_H
#include "imessage_reader.h"

namespace GWLogger::Blf
{

class CanOverloadMessageBlfReader : public IMessageReader
{
public:
	CanOverloadMessageBlfReader();
	~CanOverloadMessageBlfReader() override = default;
	BusMessagePtr read(const uint8_t* obj_begin, size_t obj_size) override;
};

};

#endif //CAN_OVERLOAD_MESSAGE_BLF_READER_H
