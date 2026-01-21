#ifndef BLF_LOGGER_CAN_ERR_MESSAGE_BLF_WRITER_H
#define BLF_LOGGER_CAN_ERR_MESSAGE_BLF_WRITER_H
#include "imessage_writer.h"
#include "blf_object_header.h"

namespace GWLogger::Blf
{

class CanErrMessageBlfWriter : public IMessageWriter
{
public:
    CanErrMessageBlfWriter();
    ~CanErrMessageBlfWriter() override = default;
    [[nodiscard]] bool write(const BusMessage& msg, FileWriter& writer) override;
    void set_timestamp_unit(TimeStampUnit unit) override;

private:
    ObjectHeaderBase header_base_{};
    ObjectHeader header_{};
};

};

#endif //BLF_LOGGER_CAN_ERR_MESSAGE_BLF_WRITER_H