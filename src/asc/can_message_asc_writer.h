#ifndef CAN_MESSAGE_ASC_MESSAGE_H
#define CAN_MESSAGE_ASC_MESSAGE_H

#include "../api/imessage_writer.h"
#include "../api/writer_registrar.h"

namespace BLF
{

class CanMessageAscWriter : public IMessageWriter
{
public:
	~CanMessageAscWriter() override = default;
	
	bool write(const BusMessage& msg, FileWriter& writer) override;
	void set_timestamp_unit(int32_t unit) override;
};

}


#endif //CAN_MESSAGE_ASC_MESSAGE_H
