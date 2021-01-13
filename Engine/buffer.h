#pragma once

namespace DXP
{

struct BufferLayout
{
    enum Type {
        Float32,
        Int32,
        UInt32,
    };

    Type type;
    int count = 0;
};

struct BufferBase
{
    virtual ~BufferBase() = default;

    virtual int ComponentSize() const = 0;

    virtual int ComponentCount() const = 0;

    virtual size_t Elements() const = 0;

    virtual const void* GetBufferPtr() const = 0;

    virtual size_t GetBufferSize() const = 0;
};

template <typename T, int COUNT = 1>
struct Buffer final : public BufferBase
{
    virtual int ComponentSize() const override {
        return sizeof(T);
    };

    virtual int ComponentCount() const override {
        return COUNT;
    };

    virtual size_t Elements() const override {
        return data.size() / COUNT;
    };

    virtual const void* GetBufferPtr() const override {
        return &data[0];
    };

    virtual size_t GetBufferSize() const override {
        return sizeof(T) * data.size();
    };

    void PushElement(T value) {
        data.push_back(value);
    };

    void PushElements(const T* ptr) {
        for (int i = 0; i < COUNT; i++)
            data.push_back(*(ptr + i));
    };

    void PushElements(const std::array<T, COUNT>& values) {
        PushElements(values.data());
    };

    T GetElement(int index) {
        return data[index];
    };

    std::array<T, COUNT> GetElements(int index) {
        return { &data[index * COUNT], &data[index * COUNT + COUNT] };
    };

    void SetElement(int index, T value) {
        data[index] = value;
    };

    void SetElements(int index, const T* ptr) {
        for (int i = 0; i < COUNT; i++)
            data[index * COUNT + i] = *(ptr + i);
    };

    void SetElements(int index, const std::array<T, COUNT>& values) {
        SetElements(index, values.data());
    };

    std::vector<T> data;
};

};
