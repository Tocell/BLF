#ifndef CANFD_MESSAGE_BLF_WRITER_H
#define CANFD_MESSAGE_BLF_WRITER_H
#include "../../api/imessage_writer.h"
#include "../../registry/writer_registrar.h"
#include "../../io/file_writer.h"

namespace BLF
{

class CanFdMessageBlfWriter : public IMessageWriter
{
public:
	CanFdMessageBlfWriter();
	~CanFdMessageBlfWriter() override = default;
	[[nodiscard]] bool write(const BusMessage& msg, FileWriter& writer) override;
	void set_timestamp_unit(int32_t unit) override;

private:
	ObjectHeaderBase header_base_{};
	ObjectHeader header_{};
};

}
#endif //CANFD_MESSAGE_BLF_WRITER_H
