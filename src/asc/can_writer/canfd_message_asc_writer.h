#ifndef CAN_MESSAGE2_ASC_WRITER_H
#define CAN_MESSAGE2_ASC_WRITER_H
#include "imessage_writer.h"
#include "writer_registrar.h"

namespace GWLogger::Asc
{

class CanFdMessageAscWriter : public IMessageWriter
{
public:
	CanFdMessageAscWriter();
	~CanFdMessageAscWriter() override = default;

	bool write(const BusMessage& msg, FileWriter& writer) override;
	void set_timestamp_unit(TimeStampUnit unit) override;

private:
	uint32_t timestamp_unit_;
};

}

#endif //CAN_MESSAGE2_ASC_WRITER_H
