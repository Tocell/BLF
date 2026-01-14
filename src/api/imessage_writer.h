#ifndef IMESSAGE_WRITER_H
#define IMESSAGE_WRITER_H

#include <iosfwd>

#include "../include/types.h"
#include "file_writer.h"

namespace BLF
{
class IMessageWriter
{
public:
	virtual ~IMessageWriter() = default;

	[[nodiscard]] virtual bool write(const BusMessage& msg, FileWriter& writer) = 0;

	virtual void set_timestamp_unit(int32_t unit) = 0;
};
}

#endif //IMESSAGE_WRITER_H
