#ifndef LOGCONTAINER_H
#define LOGCONTAINER_H
#include "object_header_base.h"
#include "abstract_file.h"
#include "common.h"

#include <iosfwd>
#include <vector>
#include <array>

BLFSTART
class TWBLF_API LogContainer : public ObjectHeaderBase
{
public:
    LogContainer();

    void read(AbstractFile& os);
    void read(LogContainer& log_container, std::fstream& file);
    void write(AbstractFile& os);

    uint16_t compression_method {};
    uint16_t reserved_log_container1 {};
    uint32_t reserved_log_container2 {};
    uint32_t uncompressed_file_size {};
    uint32_t reserved_log_container3 {};
    std::vector<uint8_t> compressed_file {};
    std::vector<uint8_t> uncompressed_file {};
    uint32_t compressed_file_size {};
    std::streampos file_position {};

    void compress(uint16_t compressionMethod, int compressionLevel);
    void uncompress();

    [[nodiscard]] uint32_t log_container_internal_header_size() const;
    [[nodiscard]] uint32_t log_container_object_size() const;

};
BLFEND

#endif //LOGCONTAINER_H
