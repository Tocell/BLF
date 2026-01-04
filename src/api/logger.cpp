#include "logger.h"
#include "logger_registry.h"

#include <iostream>

namespace BLF
{

std::unique_ptr<Logger> Logger::create(FileFormat format)
{
	return LoggerRegistry::instance().create(format);
}

}