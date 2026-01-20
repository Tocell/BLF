#ifndef READER_REGISTRAR_H
#define READER_REGISTRAR_H

#include "blf_reader_registry.h"

namespace GWLogger::Blf
{

template<typename T_Reader>
class ReaderRegistrar
{
public:
	explicit ReaderRegistrar(uint32_t object_type)
	{
		ReaderRegistry::instance().register_reader(object_type, []()
		{
			return std::make_unique<T_Reader>();
		});
	}
};

}

#endif //READER_REGISTRAR_H
