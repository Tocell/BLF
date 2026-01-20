#ifndef CANFD_MESSAGE64_BLF_WRITER_H
#define CANFD_MESSAGE64_BLF_WRITER_H
#include "imessage_writer.h"
#include "blf_object_header.h"

namespace GWLogger::Blf
{

class CanFdMessage64BlfWriter : public IMessageWriter
{
public:
	CanFdMessage64BlfWriter();
	~CanFdMessage64BlfWriter() override = default;
	[[nodiscard]] bool write(const BusMessage& msg, FileWriter& writer) override;
	void set_timestamp_unit(int32_t unit) override;

private:
	ObjectHeaderBase header_base_{};
	ObjectHeader header_{};
};

}

#endif //CANFD_MESSAGE64_BLF_WRITER_H
