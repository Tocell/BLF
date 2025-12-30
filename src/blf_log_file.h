#ifndef LOG_WRITER_H
#define LOG_WRITER_H
#include "log_file.h"
#include <fstream>
#include <memory>

class BlfLogFile : public LogFile
{
public:
	BlfLogFile() = default;

	~BlfLogFile() override;

	virtual int32_t write() = 0;

	void openFile(const std::string& filepath) override;

	void closeFile() override;

protected:
	std::ofstream file_;
};

#endif //LOG_WRITER_H
