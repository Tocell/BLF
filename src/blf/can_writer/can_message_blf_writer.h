#ifndef CAN_MESSAGE_BLF_WRITER_H
#define CAN_MESSAGE_BLF_WRITER_H
#include "imessage_writer.h"
#include "writer_registrar.h"

namespace GWLogger::Blf
{

class CanMessageBlfWriter : public IMessageWriter
{
public:
	CanMessageBlfWriter();
	~CanMessageBlfWriter() override = default;
	[[nodiscard]] bool write(const BusMessage& msg, FileWriter& writer) override;
	void set_timestamp_unit(int32_t unit) override;

private:
	ObjectHeaderBase header_base_{};
	ObjectHeader header_{};
};

}

#endif //CAN_MESSAGE_BLF_WRITER_H
