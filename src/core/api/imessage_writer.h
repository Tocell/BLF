#ifndef IMESSAGE_WRITER_H
#define IMESSAGE_WRITER_H

#include "bus_message.h"
#include "file_writer.h"

#include <cstddef>

namespace GWLogger
{
class IMessageWriter
{
public:
	virtual ~IMessageWriter() = default;

	[[nodiscard]] virtual bool write(const BusMessage& msg, FileWriter& writer) = 0;

	[[nodiscard]] virtual size_t estimate_write_size(const BusMessage& msg) const
	{
		(void)msg;
		return 400;
	}

	virtual void set_timestamp_unit(TimeStampUnit unit) = 0;
};
}

#endif //IMESSAGE_WRITER_H
