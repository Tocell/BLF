#include "file_reader.h"

namespace BLF
{

FileReader::FileReader() : file_size_(0) {}

FileReader::~FileReader()
{
	close();
}

bool FileReader::open(const std::string& file_name)
{
	close();

	filename_ = file_name;
	file_.open(file_name, std::ios::binary | std::ios::in);

	if (file_.is_open())
	{
		file_.seekg(0, std::ios::end);
		file_size_ = static_cast<uint64_t>(file_.tellg());
		file_.seekg(0, std::ios::beg);
		return true;
	}
	return false;
}

void FileReader::close()
{
	if (file_.is_open())
	{
		file_.close();
	}
	file_size_ = 0;
}

bool FileReader::is_open() const
{
	return file_.is_open();
}

bool FileReader::read(uint8_t* data, size_t size)
{
	if (!file_.is_open() || !data || 0 == size)
	{
		return false;
	}
	file_.read(reinterpret_cast<char*>(data), static_cast<std::streamsize>(size));
	const auto got = file_.gcount();
	if (got != static_cast<std::streamsize>(size))
		return false;
	return true;
}

uint64_t FileReader::tell()
{
	if (!file_.is_open())
	{
		return 0;
	}
	return file_.tellg();
}

bool FileReader::seek(uint64_t pos)
{
	if (!file_.is_open())
	{
		return false;
	}
	file_.clear();
	file_.seekg(static_cast<std::streamoff>(pos));
	return file_.good();
}

uint64_t FileReader::file_size() const
{
	return file_size_;
}

bool FileReader::eof() const
{
	return file_.eof();
}


}