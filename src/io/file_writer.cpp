#include "file_writer.h"

#include <iostream>

namespace BLF{

FileWriter::FileWriter()
	: pos_(0)
{

}

FileWriter::~FileWriter()
{
	close();
}

bool FileWriter::open(const std::string& filename, int32_t mode, bool append)
{
	close();
	filename_ = filename;
	auto fmode = mode | std::ios::out;
	if (append)
	{
		fmode |= std::ios::app;
	}

	file_.open(filename, fmode);
	return file_.is_open();
}

void FileWriter::close()
{
	if (file_.is_open())
	{
		file_.close();
	}
}

bool FileWriter::is_open() const
{
	return file_.is_open();
}

bool FileWriter::write(const uint8_t* data, size_t size)
{
	if (!file_.is_open() || !data || 0 == size)
	{
		return false;
	}
	// std::cout << "write data:" << std::endl;
	// for (auto i = 0; i < size; i++)
	// {
	// 	printf("%d ", data[i]);
	// }
	// std::cout << std::endl;
	file_.write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(size));
	if (file_.fail()) {
		std::cerr << "File write format error." << std::endl;
	}
	if (file_.bad()) {
		std::cerr << "File write hardware error." << std::endl;
	}
	return file_.good();
}

void FileWriter::append(const uint8_t* data, size_t size)
{
	memcpy(buffer_, data, size);
	pos_ += size;
}

uint64_t FileWriter::get_pos() const
{
	return pos_;
}

void FileWriter::set_pos(uint64_t pos)
{
	pos_ = pos;
}

void FileWriter::flush()
{
	if (file_.is_open())
	{
		file_.flush();
	}
}

// 定位当前文件指针位置
uint64_t FileWriter::tell()
{
	if (!file_.is_open())
	{
		return 0;
	}
	return file_.tellp();
}

bool FileWriter::seek(uint64_t pos)
{
	if (!file_.is_open())
	{
		return false;
	}
	file_.seekp(static_cast<std::streamoff>(pos));
	return file_.good();
}

uint8_t* FileWriter::get_buffer()
{
	return buffer_;
}

}
