#ifndef IMESSAGE_READER_H
#define IMESSAGE_READER_H
#include "message_factory.h"
#include <string>

namespace GWLogger
{

class IMessageReader
{
public:
	virtual ~IMessageReader() = default;

	virtual BusMessagePtr read(const uint8_t* obj_begin, size_t obj_size) = 0;
};

}
#endif //IMESSAGE_READER_H
