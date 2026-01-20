#ifndef CAN_MESSAGE2_BLF_WRITER_H
#define CAN_MESSAGE2_BLF_WRITER_H
#include "imessage_writer.h"
#include "writer_registrar.h"
#include "file_writer.h"

namespace GWLogger::Blf
{

class CanMessage2BlfWriter : public IMessageWriter
{
public:
	CanMessage2BlfWriter();
	~CanMessage2BlfWriter() override = default;
	[[nodiscard]] bool write(const BusMessage& msg, FileWriter& writer) override;
	void set_timestamp_unit(int32_t unit) override;

private:
	ObjectHeaderBase header_base_{};
	ObjectHeader header_{};
};

}
#endif //CAN_MESSAGE2_BLF_WRITER_H
