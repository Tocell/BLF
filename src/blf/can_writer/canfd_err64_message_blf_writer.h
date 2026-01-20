#ifndef BLF_LOGGER_CANFD_ERR64_MESSAGE_BLF_WRITER_H
#define BLF_LOGGER_CANFD_ERR64_MESSAGE_BLF_WRITER_H
#include "imessage_writer.h"
#include "blf_object_header.h"

namespace GWLogger::Blf
{
class CanFdErr64MessageBlfWriter : public IMessageWriter
{
public:
    CanFdErr64MessageBlfWriter();
    ~CanFdErr64MessageBlfWriter() override = default;
    [[nodiscard]] bool write(const BusMessage& msg, FileWriter& writer) override;
    void set_timestamp_unit(int32_t unit) override;

private:
    ObjectHeaderBase header_base_{};
    ObjectHeader header_{};

};
}

#endif //BLF_LOGGER_CANFD_ERR64_MESSAGE_BLF_WRITER_H