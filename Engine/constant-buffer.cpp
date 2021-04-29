#include "pch.h"

namespace DXP
{

ConstantBufferContent::ConstantBufferContent(const ConstantBufferLayout* layout) :
    layout(layout)
{
    if (layout)
        buffer.resize(layout->size);
}

void ConstantBufferContent::Write(std::string_view name, const void* data, size_t length)
{
    DXP_ASSERT(layout, "Trying to write to non existing constant buffer");

    for (const ConstantBufferField& field : layout->fields) {
        if (field.name != name)
            continue;

        DXP_ASSERT(length == field.size, "Constant buffer field length mismatch ({} != {})", length, field.size);
        memcpy(&this->buffer[0] + field.offset, data, length);
        return;
    }

    DXP::Fatal("Can't find constant buffer field {}", name);
}

};
