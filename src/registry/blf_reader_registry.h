#ifndef READER_REGISTRY_H
#define READER_REGISTRY_H
#include <memory>
#include <unordered_map>
#include <functional>
#include "imessage_reader.h"

namespace GWLogger::Blf
{
using ReaderFactory = std::function<std::unique_ptr<IMessageReader>()>;

class ReaderRegistry
{
public:
	static ReaderRegistry& instance()
	{
		static ReaderRegistry inst;
		return inst;
	}

	void register_reader(uint32_t object_type, ReaderFactory factory)
	{
		registry_[object_type] = std::move(factory);
	}

	[[nodiscard]] std::unique_ptr<IMessageReader> find_reader(uint32_t object_type) const {
		auto it = registry_.find(object_type);
		return it == registry_.end() ? nullptr : it->second();
	}

	[[nodiscard]] IMessageReader* get_singleton(uint32_t object_type) const = delete;

private:
	std::unordered_map<uint32_t, ReaderFactory> registry_;
};

}
#endif //READER_REGISTRY_H
