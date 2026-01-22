#include "file_reader.h"

namespace GWLogger
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
		return false;

	file_.read(reinterpret_cast<char*>(data), static_cast<std::streamsize>(size));
	return file_.good() && file_.gcount() == static_cast<std::streamsize>(size);
}

bool FileReader::read_line(std::string& line)
{
	line.clear();
	if (!file_.is_open())
		return false;

	// 使用 std::getline 读取到 '\n' 为止（getline 会去掉 '\n'）
	if (!std::getline(file_, line))
	{
		// 失败：可能是 EOF 且没有读到任何字符，或者真的出错
		return false;
	}

	// 如果是 CRLF，getline 会保留 '\r'，这里去掉
	if (!line.empty() && line.back() == '\r')
	{
		line.pop_back();
	}

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

bool FileReader::has_error() const
{
	return file_.bad();
}

}