#ifndef WRITER_REGISTRAR_H
#define WRITER_REGISTRAR_H

#include "writer_registry.h"

namespace BLF
{

template<typename T_Writer>
class WriterRegistrar
{
public:
	WriterRegistrar(FileFormat format, BusType bus_type)
	{
		WriterRegistry::get_instance().register_writer(format, bus_type, []()
		{
			return std::make_unique<T_Writer>();
		});
	}
};

}

#endif //WRITER_REGISTRAR_H
