#ifndef CAN_ERR_MESSAGE_BLF_READER_H
#define CAN_ERR_MESSAGE_BLF_READER_H
#include "imessage_reader.h"

namespace GWLogger::Blf
{

class CanErrMessageBlfReader : public IMessageReader
{
public:
	CanErrMessageBlfReader();
	~CanErrMessageBlfReader() override = default;
	BusMessagePtr read(const uint8_t* obj_begin, size_t obj_size) override;
};

};

#endif //CAN_ERR_MESSAGE_BLF_READER_H
