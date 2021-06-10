#pragma once

namespace DXP
{

template <typename T>
struct Codex
{
    void Set(int i, const std::shared_ptr<T>& ptr) {
        if (entries.size() < i + 1)
            entries.resize(i + 1);

        entries[i] = ptr;
    };

    T* Get(int i) {
        if (entries.size() < i + 1)
            return nullptr;

        return entries[i].get();
    };

    std::shared_ptr<T> GetSharedPtr(int i) {
        if (entries.size() < i + 1)
            return nullptr;

        return entries[i];
    };

    std::vector<T*> GetRawPtrs(int start = 0, size_t length = 0) {
        if (!length)
            length = entries.size();

        std::vector<T*> result;
        result.resize(length);

        for (int i = 0; i < length; i++)
            result[i] = Get(start + i);

        return result;
    };

    std::vector<std::shared_ptr<T>> entries;
};

};
