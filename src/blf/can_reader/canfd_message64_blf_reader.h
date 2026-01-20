#ifndef CANFD_MESSAGE64_BLF_READER_H
#define CANFD_MESSAGE64_BLF_READER_H
#include "imessage_reader.h"

namespace GWLogger::Blf
{

class CanFdMessage64BlfReader : public IMessageReader
{
public:
	CanFdMessage64BlfReader();
	~CanFdMessage64BlfReader() override = default;
	BusMessagePtr read(const uint8_t* obj_begin, size_t obj_size) override;
};

}

#endif //CANFD_MESSAGE64_BLF_READER_H
