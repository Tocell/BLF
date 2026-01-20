#ifndef WRITER_REGISTRY_H
#define WRITER_REGISTRY_H

#include <functional>

namespace GWLogger
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
	WriterRegistry& operator=(const WriterRegistry&) = delete;

	void register_writer(FileFormat format, BusType bus_type, WriterFactory factory);

	[[nodiscard]] const WriterFactory* find_writer(FileFormat format, BusType bus_type) const;

private:
	WriterRegistry() = default;
	~WriterRegistry() = default;

	std::unordered_map<uint64_t, WriterFactory> registry_{};

	static uint64_t make_key(FileFormat format, BusType bus_type);

};

inline void WriterRegistry::register_writer(FileFormat format, BusType bus_type, WriterFactory factory)
{
	registry_[make_key(format, bus_type)] = std::move(factory);
}

inline const WriterFactory* WriterRegistry::find_writer(FileFormat format, BusType bus_type) const
{
	auto it = registry_.find(make_key(format, bus_type));
	if (it == registry_.end())
		return nullptr;
	return &it->second;
}

inline uint64_t WriterRegistry::make_key(FileFormat format, BusType bus_type)
{
	// 假设 enum 都能安全转成 uint32_t
	const uint64_t f = static_cast<uint32_t>(format);
	const uint64_t b = static_cast<uint32_t>(bus_type);
	return (f << 32) | b;
}

}

#endif //WRITER_REGISTRY_H
