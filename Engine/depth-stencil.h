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
    virtual bool DepthTestEnabled() const = 0;

    virtual ~DepthStencilTest() = default;
};

struct DepthStencilTexture : public virtual Texture
{
};

};
