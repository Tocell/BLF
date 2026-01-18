#ifndef BLF_LOGGER_ASC_TYPE_MAP_MANAGER_H
#define BLF_LOGGER_ASC_TYPE_MAP_MANAGER_H

#include <map>
#include <string>
#include <mutex>

namespace BLF
{

class AscObjectTypeManager
{
public:
    AscObjectTypeManager(const AscObjectTypeManager&) = delete;
    AscObjectTypeManager& operator=(const AscObjectTypeManager&) = delete;
    AscObjectTypeManager(AscObjectTypeManager&&) = delete;
    AscObjectTypeManager& operator=(const AscObjectTypeManager&&)=delete;

    static void register_type_manager(std::string key, uint32_t type)
    {
        std::unique_lock lock(inst().mtx_);
        inst().asc_object_type_.insert({key, type});
    }

    static uint32_t get_type(std::string& key)
    {
        std::unique_lock lock(inst().mtx_);
        auto it = inst().asc_object_type_.find(key);
        if (it == inst().asc_object_type_.end())
            return -1;
        return it->second;
    }

private:
    AscObjectTypeManager() {}

    ~AscObjectTypeManager() = default;

    static AscObjectTypeManager& inst()
    {
        static AscObjectTypeManager manager;
        return manager;
    }

    std::map<std::string, uint32_t> asc_object_type_{};
    std::mutex mtx_{};
};

}

#endif //BLF_LOGGER_ASC_TYPE_MAP_MANAGER_H