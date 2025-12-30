#ifndef LOG_STRATEGY_H
#define LOG_STRATEGY_H
#include <string>

class LogFile
{
public:
	LogFile() = default;
	virtual ~LogFile() = default;

	virtual void openFile(const std::string& filepath) = 0;
	virtual void closeFile() = 0;
};

#endif //LOG_STRATEGY_H
