#pragma once

namespace DXP
{

enum class BufferFormat : int 
{
    Float32_4,
    Float32_3,
    Float32_2,
    Float32_1,
    Uint16_1,
    Uint32_1,
};

template <BufferFormat T> struct BufferProperties;

template <> struct BufferProperties<BufferFormat::Float32_4>
{
    using underlying_type = float32_t;
    static constexpr int stride = sizeof(float32_t) * 4;
};

template <> struct BufferProperties<BufferFormat::Float32_3>
{
    using underlying_type = float32_t;
    static constexpr int stride = sizeof(float32_t) * 3;
};

template <> struct BufferProperties<BufferFormat::Float32_2>
{
    using underlying_type = float32_t;
    static constexpr int stride = sizeof(float32_t) * 2;
};

template <> struct BufferProperties<BufferFormat::Float32_1>
{
    using underlying_type = float32_t;
    static constexpr int stride = sizeof(float32_t) * 1;
};

template <> struct BufferProperties<BufferFormat::Uint16_1>
{
    using underlying_type = uint16_t;
    static constexpr int stride = sizeof(uint16_t) * 1;
};

template <> struct BufferProperties<BufferFormat::Uint32_1>
{
    using underlying_type = uint32_t;
    static constexpr int stride = sizeof(uint32_t) * 1;
};

struct BufferBase
{
    virtual ~BufferBase() = default;

    virtual BufferFormat Format() const = 0;

    virtual uint32_t Stride() const = 0;

    virtual const void* GetBufferPtr() const = 0;

    virtual size_t GetBufferSize() const = 0;

    virtual size_t Elements() const = 0;
};

template <BufferFormat T>
struct Buffer : public BufferBase
{
    BufferFormat Format() const override {
        return T;
    };

    virtual uint32_t Stride() const override {
        return BufferProperties<T>::stride;
    };

    virtual const void* GetBufferPtr() const override {
        return static_cast<const void *>(&data[0]);
    };

    virtual size_t GetBufferSize() const override {
        return data.size() * sizeof(typename BufferProperties<T>::underlying_type);
    };

    virtual size_t Elements() const override {
        return data.size();
    };

    void PushElement(typename BufferProperties<T>::underlying_type value) {
        data.push_back(value);
    };

    std::vector<typename BufferProperties<T>::underlying_type> data;
};

};
