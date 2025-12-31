#ifndef BLF_FILE_H
#define BLF_FILE_H
#include <memory>

#include "../io/file_writer.h"

namespace BLF
{

class BlfFileWriter
{
public:
	BlfFileWriter();
	~BlfFileWriter();

	bool open(const std::string& filepath, bool append = false);

	void close();

	[[nodiscard]] bool is_open() const;

private:
	std::unique_ptr<FileWriter> writer_;

	bool has_writeen_header_;
};

}

#endif //BLF_FILE_H
