#pragma once

namespace DXP
{

struct DirectX11Viewport : public Viewport
{
    DirectX11Viewport(int x, int y, int width, int height) {
        viewport.TopLeftX = static_cast<float>(x);
        viewport.TopLeftY = static_cast<float>(y);
        viewport.Width = static_cast<float>(width);
        viewport.Height = static_cast<float>(height);
        viewport.MinDepth = 0.f;
        viewport.MaxDepth = 1.f;
    };

    virtual int GetX() const {
        return static_cast<int>(viewport.TopLeftX);
    };

    virtual int GetY() const {
        return static_cast<int>(viewport.TopLeftY);
    };

    virtual int GetWidth() const {
        return static_cast<int>(viewport.Width);
    };

    virtual int GetHeight() const {
        return static_cast<int>(viewport.Height);
    };

    D3D11_VIEWPORT viewport;
};

};
