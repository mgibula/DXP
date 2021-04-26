#pragma once

namespace DXP
{

struct Material
{
    std::shared_ptr<VertexShader> vertexShader;
    std::shared_ptr<PixelShader> pixelShader;

    std::array<std::shared_ptr<ConstantBuffer>, static_cast<int>(ConstantBufferSlot::_Count)> constantBuffers;
    // textures
};

};
