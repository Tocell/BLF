#ifndef CAN_DRIVER_ERR_MESSAGE_BLF_READER_H
#define CAN_DRIVER_ERR_MESSAGE_BLF_READER_H
#include "imessage_reader.h"

namespace GWLogger::Blf
{

class CanDriverErrMessageBlfReader : public IMessageReader
{
public:
	CanDriverErrMessageBlfReader();
	~CanDriverErrMessageBlfReader() override = default;
	BusMessagePtr read(const uint8_t* obj_begin, size_t obj_size) override;
};

};

#endif //CAN_DRIVER_ERR_MESSAGE_BLF_READER_H
