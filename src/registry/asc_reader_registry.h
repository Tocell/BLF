#ifndef BLF_LOGGER_ASC_READER_REGISTRY_H
#define BLF_LOGGER_ASC_READER_REGISTRY_H
#include <memory>
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>

#include "iasc_message_reader.h"
#include "asc_reader_helper.h"

namespace GWLogger::Asc
{

enum class AscLineKey : uint32_t {
    Unknown = 0,
    CanClassic,
    CanFd,
    // CanError,
    // CanStatistic,
    // CanOverload,
    // ...
};

struct AscLineKeyHash {
    size_t operator()(AscLineKey k) const noexcept {
        return static_cast<size_t>(k);
    }
};

using AscReaderFactory = std::function<std::unique_ptr<IAscMessageReader>()>;

class AscReaderRegistry
{
public:
    static AscReaderRegistry& instance()
    {
        static AscReaderRegistry inst;
        return inst;
    }

    void registry_reader(AscLineKey key, AscReaderFactory f)
    {
        registry_[key].push_back(std::move(f));
    }

    static inline AscLineKey asc_extract_key(const std::string& line)
    {
        if (line.empty()) return AscLineKey::Unknown;
        if (line.size() >= 2 && line[0] == '/' && line[1] == '/') return AscLineKey::Unknown;

        auto t = split_ws(line);
        if (t.size() < 2) return AscLineKey::Unknown;

        // CANFD: <Time> CANFD ...
        if (t[1] == "CANFD") return AscLineKey::CanFd;

        // Classic CAN: <Time> <Ch> <ID> <Dir> <d/r> ...
        if (t.size() >= 5 &&
            (t[3] == "Tx" || t[3] == "Rx" || t[3] == "TxRq") &&
            (t[4] == "d"  || t[4] == "r"))
            return AscLineKey::CanClassic;

        return AscLineKey::Unknown;
    }


    IAscMessageReader* find_reader(
        const std::string& line,
        std::unordered_map<AscLineKey,
            std::vector<std::unique_ptr<IAscMessageReader>>,
            AscLineKeyHash>& cache) const
    {
        AscLineKey key = asc_extract_key(line);
        if (key == AscLineKey::Unknown) return nullptr;

        // 先查 cache：该 key 已经实例化过 reader
        auto& vec = cache[key];
        if (!vec.empty())
            return vec.front().get();

        // 再查 registry：创建 reader 实例放入 cache
        auto it = registry_.find(key);
        if (it == registry_.end()) return nullptr;

        for (auto& fac : it->second)
        {
            vec.push_back(fac());
            if (vec.back()->match(line))
                return vec.back().get();
        }
        return nullptr;
    }

private:
    std::unordered_map<AscLineKey, std::vector<AscReaderFactory>, AscLineKeyHash> registry_;
};

}

#endif //BLF_LOGGER_ASC_READER_REGISTRY_H