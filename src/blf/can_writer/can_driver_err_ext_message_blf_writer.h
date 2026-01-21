#ifndef CAN_DRIVER_ERR_EXT_MESSAGE_BLF_WRITER_H
#define CAN_DRIVER_ERR_EXT_MESSAGE_BLF_WRITER_H
#include "imessage_writer.h"
#include "blf_object_header.h"

namespace GWLogger::Blf
{

class CanDriverErrExtMessageBlfWriter : public IMessageWriter
{
public:
	CanDriverErrExtMessageBlfWriter();
	~CanDriverErrExtMessageBlfWriter() override = default;
	[[nodiscard]] bool write(const BusMessage& msg, FileWriter& writer) override;
	void set_timestamp_unit(int32_t unit) override;

private:
	ObjectHeaderBase header_base_{};
	ObjectHeader header_{};
};

}

#endif //CAN_DRIVER_ERR_EXT_MESSAGE_BLF_WRITER_H
