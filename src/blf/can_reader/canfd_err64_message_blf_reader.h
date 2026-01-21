#ifndef CANFD_ERR64_MESSAGE_BLF_READER_H
#define CANFD_ERR64_MESSAGE_BLF_READER_H
#include "imessage_reader.h"

namespace GWLogger::Blf
{

class CanErr64MessageBlfReader : public IMessageReader
{
public:
	CanErr64MessageBlfReader();
	~CanErr64MessageBlfReader() override = default;
	BusMessagePtr read(const uint8_t* obj_begin, size_t obj_size) override;
};

};

#endif //CANFD_ERR64_MESSAGE_BLF_READER_H
