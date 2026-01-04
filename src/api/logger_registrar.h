#ifndef LOGGER_REGISTRAR_H
#define LOGGER_REGISTRAR_H

#include "logger_registry.h"
namespace BLF
{

template<class T>
class LoggerRegistrar
{
public:
	explicit LoggerRegistrar(FileFormat format)
	{
		LoggerRegistry::instance().register_logger(format, []()
		{
			return std::make_unique<T>();
		});
	}
};

}
#endif //LOGGER_REGISTRAR_H
