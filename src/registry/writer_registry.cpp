// #include "writer_registry.h"
//
// namespace BLF
// {
//
// void WriterRegistry::register_writer(FileFormat format, BusType bus_type, WriterFactory factory)
// {
// 	registry_[make_key(format, bus_type)] = std::move(factory);
// }
//
// const WriterFactory* WriterRegistry::find_writer(FileFormat format, BusType bus_type) const
// {
// 	auto it = registry_.find(make_key(format, bus_type));
// 	if (it == registry_.end())
// 		return nullptr;
// 	return &it->second;
// }
//
// }
//
