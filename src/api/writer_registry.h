#ifndef WRITER_REGISTRY_H
#define WRITER_REGISTRY_H

#include <functional>
#include <map>
#include <memory>

#include "imessage_writer.h"
#include "../include/types.h"

namespace BLF
{

using WriterFactory = std::function<std::unique_ptr<IMessageWriter>()>;

class WriterRegistry {
public:
	static WriterRegistry& get_instance()
	{
		static WriterRegistry instance;
		return instance;
	}

	WriterRegistry(const WriterRegistry&) = delete;
	void operator=(const WriterRegistry&) = delete;

	void register_writer(FileFormat format, BusType bus_type, WriterFactory factory);

	std::map<BusType, std::unique_ptr<IMessageWriter>> create_writers(FileFormat format);

private:
	WriterRegistry() = default;
	~WriterRegistry() = default;

	std::map<FileFormat, std::map<BusType, WriterFactory>> registry_{};
};

}

#endif //WRITER_REGISTRY_H
