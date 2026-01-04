#ifndef CAN_MESSAGE_BLF_WRITER_H
#define CAN_MESSAGE_BLF_WRITER_H
#include "../api/imessage_writer.h"
#include "../api/writer_registrar.h"

namespace BLF
{

class CanMessageBlfWriter : public IMessageWriter
{
public:
	[[nodiscard]] bool write(const BusMessage& msg, FileWriter& writer) const override;
};

}

#endif //CAN_MESSAGE_BLF_WRITER_H
