#pragma once

namespace DXP
{

enum RasterizerSlot {
    Rasterizer_Solid,
    Rasterizer_Wireframe,
};

struct RasterizerSettings
{
    bool wireframe = false;
    bool drawFront = true;
    bool drawBack = false;
};

};
