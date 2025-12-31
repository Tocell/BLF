#ifndef IMESSAGE_WRITER_H
#define IMESSAGE_WRITER_H

#include <iosfwd>

#include "../include/types.h"

namespace BLF
{
class IMessageWriter
{
public:
	virtual ~IMessageWriter() = default;

	virtual bool write(const BusMessage& msg, std::ostream& oout_stream) const = 0;
};
}

#endif //IMESSAGE_WRITER_H
