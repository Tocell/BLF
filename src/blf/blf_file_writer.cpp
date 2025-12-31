#include "blf_file_writer.h"

namespace BLF
{

BlfFileWriter::BlfFileWriter()
	: writer_(nullptr),
	has_writeen_header_(false)
{

}

BlfFileWriter::~BlfFileWriter()
{
	close();
}

bool BlfFileWriter::open(const std::string& filepath, bool append)
{
	close();

	writer_ = std::make_unique<FileWriter>();
	if (!writer_->open(filepath, append))
	{
		return false;
	}
	return writer_->is_open();
}

bool BlfFileWriter::is_open() const
{
	return writer_ && writer_->is_open();
}

void BlfFileWriter::close()
{
	if (writer_ && writer_->is_open())
	{
		// todo 更新文件头部的 file_statistics
		writer_->flush();
	}
	if (writer_)
	{
		writer_->close();
		writer_.reset();
	}
}

}

