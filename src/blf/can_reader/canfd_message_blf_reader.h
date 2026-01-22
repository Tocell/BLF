#ifndef CANFD_MESSAGE_BLF_READER_H
#define CANFD_MESSAGE_BLF_READER_H
#include "imessage_reader.h"

namespace GWLogger::Blf
{

class CanFdMessageBlfReader : public IMessageReader
{
public:
	CanFdMessageBlfReader();
	~CanFdMessageBlfReader() override = default;
	BusMessagePtr read(const uint8_t* obj_begin, size_t obj_size) override;
};

}
#endif //CANFD_MESSAGE_BLF_READER_H
