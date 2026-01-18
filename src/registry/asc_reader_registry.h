#ifndef BLF_LOGGER_ASC_READER_REGISTRY_H
#define BLF_LOGGER_ASC_READER_REGISTRY_H
#include <memory>
#include <functional>

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

    void registry_reader(AscReaderFactory f)
    {
        registry_.push_back(std::move(f));
    }

    IAscMessageReader* pick_reader(const std::string& line,
        std::vector<std::unique_ptr<IAscMessageReader>>& cache) const
    {
        for (auto& r : cache)
        {
            if (r->match(line))
                return r.get();
        }

        for (auto&f : registry_)
        {
            cache.push_back(f());
            if (cache.back()->match(line))
                return cache.back().get();
        }
        return nullptr;
    }

private:
    std::vector<AscReaderFactory> registry_;
};

}

#endif //BLF_LOGGER_ASC_READER_REGISTRY_H