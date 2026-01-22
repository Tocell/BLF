#ifndef CAN_DRIVER_ERR_EXT_MESSAGE_BLF_READER_H
#define CAN_DRIVER_ERR_EXT_MESSAGE_BLF_READER_H
#include "imessage_reader.h"

namespace GWLogger::Blf
{

class CanDriver_errExtMessageBlfReader : public IMessageReader
{
public:
	CanDriver_errExtMessageBlfReader();
	~CanDriver_errExtMessageBlfReader() override = default;
	BusMessagePtr read(const uint8_t* obj_begin, size_t obj_size) override;
};

};

#endif //CAN_DRIVER_ERR_EXT_MESSAGE_BLF_READER_H
