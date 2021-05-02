#pragma once

namespace DXP
{

enum ConstantBufferSlot 
{
    // Order is important - materials are starting their bindings
    // from CB_Material upwards
    CB_Transform           = 0,
    CB_Camera              = 1,
    CB_Frame               = 2,
    CB_Material            = 3,
    CB_Object              = 4,
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
