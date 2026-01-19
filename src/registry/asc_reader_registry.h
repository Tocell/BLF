#ifndef BLF_LOGGER_ASC_READER_REGISTRY_H
#define BLF_LOGGER_ASC_READER_REGISTRY_H
#include <memory>
#include <functional>
#include <unordered_map>

#include "../api/iasc_message_reader.h"

namespace BLF
{

using AscReaderFactory = std::function<std::unique_ptr<IAscMessageReader>()>;

class AscReaderRegistry
{
public:
    static AscReaderRegistry& instance()
    {
        static AscReaderRegistry inst;
        return inst;
    }

    void registry_reader(uint32_t key, AscReaderFactory f)
    {
        registry_[key] = std::move(f);
    }

    IAscMessageReader* create(
        uint32_t key,
        std::unordered_map<uint32_t, std::unique_ptr<IAscMessageReader>>& cache) const
    {
        if (auto itc = cache.find(key); itc != cache.end())
            return itc->second.get();

        // 再查 registry
        auto itr = registry_.find(key);
        if (itr == registry_.end())
            return nullptr;

        auto [it, inserted] = cache.emplace(key, itr->second());
        return it->second.get();
    }

private:
    std::unordered_map<uint32_t, AscReaderFactory> registry_;
};

}

#endif //BLF_LOGGER_ASC_READER_REGISTRY_H