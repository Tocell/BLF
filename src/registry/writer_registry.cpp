#include "writer_registry.h"

namespace BLF
{

void WriterRegistry::register_writer(FileFormat format, BusType bus_type, WriterFactory factory)
{
	registry_[format][bus_type] = std::move(factory);
}

std::map<BusType, std::unique_ptr<IMessageWriter>> WriterRegistry::create_writers(FileFormat format)
{
	std::map<BusType, std::unique_ptr<IMessageWriter>> writers;
	if (registry_.count(format))
	{
		for (const auto& pair : registry_.at(format))
		{
			writers[pair.first] = pair.second();  // 调用了 WriterFactory（）
		}
	}
	return writers;
}

}

