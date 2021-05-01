#include "pch.h"

namespace DXP
{

Material::Material(RenderBackend *gpu, std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<PixelShader> pixelShader) :
    vertexShader(vertexShader),
    pixelShader(pixelShader)
{
    uint64_t slots = gpu->GetLimitValue(RenderBackend::Limit::ConstantBufferSlots);
    constantBuffers.resize(slots);
    constantBufferLayouts.resize(slots);

    for (int i = 0; i < slots - 1; i++) {
        // Transform slot is hardcoded
        if (i == ConstantBufferSlot::CB_Transform)
            continue;

        FillConstantBufferFromShader(gpu, vertexShader.get(), i);
        FillConstantBufferFromShader(gpu, pixelShader.get(), i);
    }

    constantBufferPerMaterial = CreateConstantBufferData(ConstantBufferSlot::CB_Material);
}

void Material::FillConstantBufferFromShader(RenderBackend* gpu, Shader* shader, int slot)
{
    if (!shader)
        return;

    const ConstantBufferLayout* layout = shader->GetConstantBufferLayout(slot);
    if (!layout || !layout->size)
        return;

    constantBufferLayouts[slot] = layout;

    // We're assuming that all shaders in material shares the same constant buffer structures
    if (constantBuffers[slot]) {
        DXP_ASSERT(constantBuffers[slot]->GetSize() == layout->size, "Constant buffer size mismatch for shader (slot {}, {} != {})", slot, constantBuffers[slot]->GetSize(), layout->size);
    } else {
        constantBuffers[slot] = gpu->CreateConstantBuffer(layout->size);
    }
}

std::unique_ptr<ConstantBufferContent> Material::CreateConstantBufferData(int slot)
{
    DXP_ASSERT(slot < constantBufferLayouts.size(), "Trying to read buffer outsize of available limit (limit: {}, slot: {})", constantBufferLayouts.size(), slot);
    
    if (constantBufferLayouts[slot])
        return std::make_unique<ConstantBufferContent>(constantBufferLayouts[slot]);

    return nullptr;
}

};
