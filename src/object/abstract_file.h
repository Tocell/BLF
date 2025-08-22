#ifndef ABSTRACTFILE_H
#define ABSTRACTFILE_H
#include "common.h"
#include "object_header_base.h"
#include <ios>
#include <vector>

BLFSTART

class TWBLF_API AbstractFile {
public:
    AbstractFile() noexcept = default;
    virtual ~AbstractFile() noexcept = default;
    DISALLOW_COPY_AND_ASSIGN(AbstractFile)

    [[nodiscard]] virtual std::streamsize gcount() const = 0;
    virtual void read(char * s, std::streamsize n) = 0;
    virtual std::streampos tellg() = 0;
    virtual void seekg(std::streamoff off, std::ios_base::seekdir way) = 0;
    virtual void write(const char * s, std::streamsize n) = 0;
    virtual std::streampos tellp() = 0;
    [[nodiscard]] virtual bool good() const = 0;
    [[nodiscard]] virtual bool eof() const = 0;
    virtual void serialize(ObjectHeaderBase* ohb, uint32_t& offset) {};
    virtual void skipp(std::streamsize s) final
    {
        std::vector<char> zero;
        zero.resize(s);
        write(zero.data(), s);
    }
};

BLFEND
#endif //ABSTRACTFILE_H
