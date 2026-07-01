#ifndef FILE_H
#define FILE_H
#include <string>
#include <fstream>
#include <cstdint>
#include <array>

#include "internal_define.h"

namespace GWLogger
{

class FileWriter
{
public:
	FileWriter();
	~FileWriter();

	bool open(const std::string& filename);

	void close();

	[[nodiscard]] bool is_open() const;

	bool write(const uint8_t* data, size_t size);

	template<typename T>
	int32_t write_struct(const T& obj)
	{
		return write(reinterpret_cast<const uint8_t*>(&obj), sizeof(T));
	}

	void append(const uint8_t* data, size_t size);

	void flush();

	// 定位当前文件指针位置
	uint64_t tell();

	bool seek(uint64_t pos);

	[[nodiscard]] uint64_t get_pos() const;

	void set_pos(uint64_t pos);

	uint8_t* get_buffer();

	void set_file_start_time(uint64_t time)
	{
		file_start_time_ = time;
	}

	[[nodiscard]] uint64_t get_file_start_time() const
	{
		return file_start_time_;
	}

private:
	std::string filename_{};
	std::ofstream file_{};
	uint64_t file_start_time_{};

	uint64_t pos_;
	std::array<uint8_t, BUFFER_MAX_SIZE> buffer_{};
};

}

#endif //FILE_H
