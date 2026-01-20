#ifndef BLF_LOGGER_ASC_READER_REGISTRAR_H
#define BLF_LOGGER_ASC_READER_REGISTRAR_H

#include "asc_reader_registry.h"

namespace GWLogger::Asc
{

template<typename T>
class AscReaderRegistrar {
public:
    AscReaderRegistrar() {
        AscReaderRegistry::instance().registry_reader(T::kKey, []{
            return std::make_unique<T>();
        });
    }
};

}
#endif //BLF_LOGGER_ASC_READER_REGISTRAR_H