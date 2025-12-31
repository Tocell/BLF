#ifndef FILE_READER_H
#define FILE_READER_H

#include <fstream>
#include <string>

namespace BLF
{

class FileReader
{
public:
	FileReader();
	~FileReader();

	bool open(const std::string& file_name);

	void close();

	[[nodiscard]] bool is_open() const;

	bool read(uint8_t* data, size_t size);

	template<typename T>
	bool read_struct(T& obj)
	{
		return read(reinterpret_cast<uint8_t*>(&obj), sizeof(T));
	}

	uint64_t tell();

	bool seek(uint64_t pos);

	[[nodiscard]] uint64_t file_size() const;

	[[nodiscard]] bool eof() const;


private:
	std::string filename_;
	std::ifstream file_;
	uint64_t file_size_;
};

}
#endif //FILE_READER_H
