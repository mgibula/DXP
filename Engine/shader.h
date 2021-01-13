#pragma once

namespace DXP
{

struct VertexShader
{
    virtual ~VertexShader() = default;

    virtual std::vector<BufferLayout> GetInputLayout() = 0;
    virtual std::string DebugName() const = 0;
};

struct PixelShader
{
    virtual ~PixelShader() = default;

    virtual std::string DebugName() const = 0;
};

};
