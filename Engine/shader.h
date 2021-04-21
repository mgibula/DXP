#pragma once

namespace DXP
{

enum class VertexShaderInput : int
{
    Position0,
    Position1,
    Position2,
    Position3,
    Color0,
    Color1,
    Color2,
    Color3,
    UV0,
    UV1,
    UV2,
    UV3,
    _Count,             // Always last
};

struct VertexShaderInputLayout
{
    int elements = 0;
    std::array<VertexShaderInput, 16> semantics;
    std::array<BufferFormat, 16> formats;

    void PushElement(VertexShaderInput semantic, BufferFormat format) {
        semantics[elements] = semantic;
        formats[elements] = format;
        elements++;
    };

    bool operator==(const VertexShaderInputLayout& rhs) const noexcept {
        return (
            elements == rhs.elements &&
            !memcmp(&semantics[0], &rhs.semantics[0], elements * sizeof(VertexShaderInputLayout)) &&
            !memcmp(&formats[0], &rhs.formats[0], elements * sizeof(BufferFormat))
        );
    };

    bool operator!=(const VertexShaderInputLayout& rhs) const noexcept {
        return !(*this == rhs);
    };

    bool operator<(const VertexShaderInputLayout& rhs) const noexcept {
        if (elements != rhs.elements)
            return elements < rhs.elements;

        for (int i = 0, max = elements; i < max; i++) {
            if (semantics[i] != rhs.semantics[i])
                return (semantics[i] < rhs.semantics[i]);

            if (formats[i] != rhs.formats[i])
                return (formats[i] < rhs.formats[i]);
        }

        return false;
    };
};

struct VertexShader
{
    virtual ~VertexShader() = default;

    virtual const std::vector<VertexShaderInput>& GetInputFormat() const = 0;
    virtual const ConstantBufferLayout* GetConstantBufferLayout(ConstantBufferSlot slot) const = 0;
    virtual std::string DebugName() const = 0;
};

struct PixelShader
{
    virtual ~PixelShader() = default;

    virtual const ConstantBufferLayout* GetConstantBufferLayout(ConstantBufferSlot slot) const = 0;
    virtual std::string DebugName() const = 0;
};

};
