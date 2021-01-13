#pragma once

namespace DXP
{

struct VertexShader
{
    virtual ~VertexShader() = default;

    virtual std::vector<BufferLayout> GetInputLayout() = 0;
};

struct PixelShader
{
    virtual ~PixelShader() = default;
};

};
