#include "logger.h"
#include "logger_registry.h"

namespace GWLogger
{

std::unique_ptr<Logger> Logger::create(FileFormat format)
{
	return LoggerRegistry::instance().create(format);
}

}