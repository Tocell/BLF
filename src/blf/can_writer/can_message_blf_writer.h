#ifndef CAN_MESSAGE_BLF_WRITER_H
#define CAN_MESSAGE_BLF_WRITER_H
#include "../../api/imessage_writer.h"
#include "../../registry/writer_registrar.h"

namespace BLF
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
