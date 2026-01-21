#ifndef CAN_ERR_EXT_MESSAGE_BLF_READER_H
#define CAN_ERR_EXT_MESSAGE_BLF_READER_H
#include "imessage_reader.h"

namespace GWLogger::Blf
{

class CanErrExtMessageBlfReader : public IMessageReader
{
public:
	CanErrExtMessageBlfReader();
	~CanErrExtMessageBlfReader() override = default;
	BusMessagePtr read(const uint8_t* obj_begin, size_t obj_size) override;
};

};

#endif //CAN_ERR_EXT_MESSAGE_BLF_READER_H
