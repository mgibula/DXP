#pragma once

namespace DXP
{

enum class ConstantBufferSlot : int
{
    Transform           = 0,
    Camera              = 1,             
    Frame               = 2,
    Material            = 3,
    Object              = 4,
    _Count,             // Always last
};

struct ConstantBufferField
{
    std::string name;
    int16_t size = 0;
    int16_t offset = 0;
};

struct ConstantBufferLayout
{
    size_t size = 0;
    std::vector<ConstantBufferField> fields;
};

struct ConstantBufferContent
{
    ConstantBufferContent(const ConstantBufferLayout* layout);

    template <typename T>
    void Write(std::string_view name, T&& value) {
        Write(name, static_cast<const void*>(&value), sizeof(T));
    };

    void Write(std::string_view name, const void* data, size_t length);

    const ConstantBufferLayout *layout;
    std::vector<char> buffer;
};

};
