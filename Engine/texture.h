#pragma once

namespace DXP
{

struct Texture
{
    virtual ~Texture() = default;

    virtual int Channels() const = 0;

    virtual uint32_t Width() const = 0;

    virtual uint32_t Height() const = 0;

    virtual void *GetImGuiImage() const = 0;
};

struct RenderTexture : public virtual Texture, public virtual RenderTarget
{
};

};
