#pragma once

namespace DXP
{

enum class DepthStencil
{
    Enabled,
    Disabled,
};

struct DepthStencilTest
{
    virtual ~DepthStencilTest() = default;
};

};
