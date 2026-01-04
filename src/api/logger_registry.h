#ifndef LOGGER_REGISTRY_H
#define LOGGER_REGISTRY_H
#include <functional>
#include <map>
#include <memory>

#include "../include/logger.h"

namespace BLF
{

class LoggerRegistry
{
public:
	LoggerRegistry(const LoggerRegistry&) = delete;
	void operator=(const LoggerRegistry&) = delete;

	using CreatorFunc = std::function<std::unique_ptr<Logger>()>;

	static LoggerRegistry& instance()
	{
		static LoggerRegistry instance;
		return instance;
	}

	void register_logger(FileFormat format, CreatorFunc creator)
	{
		registry_[format] = std::move(creator);
	}

	std::unique_ptr<Logger> create(FileFormat format)
	{
		if (registry_.find(format) != registry_.end())
		{
			return registry_[format]();
		}
		return nullptr;
	}

private:
	LoggerRegistry() = default;
	~LoggerRegistry() = default;

	std::map<FileFormat, CreatorFunc> registry_;
};

}

#endif //LOGGER_REGISTRY_H
