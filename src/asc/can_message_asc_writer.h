#ifndef CAN_MESSAGE_ASC_MESSAGE_H
#define CAN_MESSAGE_ASC_MESSAGE_H

#include "../api/imessage_writer.h"
#include "../api/writer_registrar.h"

namespace BLF
{

class CanMessageAscWriter : public IMessageWriter
{
public:
	bool write(const BusMessage& msg, FileWriter& writer) override;
};

}


#endif //CAN_MESSAGE_ASC_MESSAGE_H
