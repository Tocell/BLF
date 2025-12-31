#ifndef CAN_MESSAGE_BLF_WRITER_H
#define CAN_MESSAGE_BLF_WRITER_H
#include "imessage_writer.h"

namespace BLF
{
	class CanMessageWriter : public IMessageWriter
	{
	public:
		bool write(const BusMessage& msg, std::ostream& oout_stream) const override;
	};
}

#endif //CAN_MESSAGE_BLF_WRITER_H
