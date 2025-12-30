#ifndef LOG_READ_H
#define LOG_READ_H
#include <cstdint>
#include <fstream>

#include "log.h"
#include "log_file.h"

class AscLogFile : public LogFile
{
public:
	AscLogFile() = default;

	~AscLogFile() override
	{
		if (file_.is_open())
		{
			file_.close();
		}
	}

	void openFile(const std::string& filepath) override
	{
		if (file_.is_open())
		{
			file_.close();
		}
		file_.open(filepath, std::ios::app);
		if (!file_.is_open())
		{
			LOG_ERROR("Openfile file {} failed.", filepath.c_str());
		}
	}

	void closeFile() override
	{
		if (file_.is_open())
		{
			file_.close();
		}
	}

protected:
	std::ofstream file_{};
};

#endif //LOG_READ_H
