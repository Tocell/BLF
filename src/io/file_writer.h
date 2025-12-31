#ifndef FILE_H
#define FILE_H

#include <string>
#include <fstream>
#include <cstdint>

namespace BLF
{

class FileWriter
{
public:
	FileWriter();
	~FileWriter();

	bool open(const std::string& filename, bool append = false);

	void close();

	[[nodiscard]] bool is_open() const;

	bool write(const uint8_t* data, size_t size);

	template<typename T>
	int32_t write_struct(const T& obj)
	{
		return write(reinterpret_cast<const uint8_t*>(&obj), sizeof(T));
	}

	void flush();

	// 定位当前文件指针位置
	uint64_t tell();

	bool seek(uint64_t pos);


private:
	std::ofstream file_;
	std::string filename_;
};

}

#endif //FILE_H
