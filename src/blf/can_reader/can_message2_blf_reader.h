#ifndef CAN_MESSAGE2_BLF_READER_H
#define CAN_MESSAGE2_BLF_READER_H
#include "imessage_reader.h"

namespace GWLogger::Blf
{

class CanMessage2BlfReader : public IMessageReader
{
public:
	CanMessage2BlfReader();
	~CanMessage2BlfReader() override = default;
	BusMessagePtr read(const uint8_t* obj_begin, size_t obj_size) override;
};

}

#endif //CAN_MESSAGE2_BLF_READER_H
