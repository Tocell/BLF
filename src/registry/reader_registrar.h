#ifndef READER_REGISTRAR_H
#define READER_REGISTRAR_H
#include <fmt/format.h>

#include "reader_registry.h"

namespace BLF
{

template<typename T_Reader>
class ReaderRegistrar
{
public:
	ReaderRegistrar(uint32_t object_type)
	{
		ReaderRegistry::instance().register_reader(object_type, []()
		{
			return std::make_unique<T_Reader>();
		});
	}
};

}

#endif //READER_REGISTRAR_H
