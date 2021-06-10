#pragma once

namespace DXP
{

template <typename LOADER, typename KEY, typename RESOURCE>
struct Loader
{
    std::shared_ptr<RESOURCE> Set(const KEY& key, const std::shared_ptr<RESOURCE>& ptr) {
        entries[key] = ptr;
        return ptr;
    };

    std::shared_ptr<RESOURCE> GetSharedPtr(const KEY& key) {
        auto i = entries.find(key);
        if (i == entries.end())
            return nullptr;

        return i->second;
    };

    RESOURCE* GetRawPtr(const KEY& key) {
        auto result = GetSharedPtr(key);
        if (result)
            return result.get();

        return nullptr;
    };

    template <typename... Args>
    std::shared_ptr<RESOURCE> Load(Args&&... params) {
        return Load(KEY{ std::forward<Args>(params)... });
    };

    std::shared_ptr<RESOURCE> Load(KEY&& key) {
        auto result = GetSharedPtr(key);
        if (result)
            return result;

        return Set(key, static_cast<LOADER*>(this)->LoadResource(key));
    };

    std::unordered_map<KEY, std::shared_ptr<RESOURCE>, typename KEY::Hash> entries;
};

};
