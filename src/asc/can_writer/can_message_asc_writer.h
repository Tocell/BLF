#ifndef CAN_MESSAGE_ASC_MESSAGE_H
#define CAN_MESSAGE_ASC_MESSAGE_H

#include "imessage_writer.h"
#include "writer_registrar.h"

namespace GWLogger::Asc
{

class CanMessageAscWriter : public IMessageWriter
{
public:
	CanMessageAscWriter();
	~CanMessageAscWriter() override = default;

	bool write(const BusMessage& msg, FileWriter& writer) override;
	void set_timestamp_unit(int32_t unit) override;

private:
	uint32_t timestamp_unit_;
};

}

#endif //CAN_MESSAGE_ASC_MESSAGE_H
