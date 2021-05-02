#include "pch.h"

namespace DXP
{

Renderer::Renderer(std::shared_ptr<spdlog::logger> log) :
    scene(std::make_unique<SceneNode>()),
    log(log)
{
    SPDLOG_LOGGER_INFO(log, "Initializing");
}

void Renderer::SetRenderBackend(RenderBackend* backend)
{
    using namespace DirectX;

    SPDLOG_LOGGER_INFO(log, "Using renderer backend: {}", backend->InfoString());
    gpu = backend;

    transformConstantBuffer = gpu->CreateConstantBuffer(sizeof(XMFLOAT4X4));
}

std::shared_ptr<VertexShader> Renderer::LoadVertexShader(std::string_view path)
{
    std::shared_ptr<VertexShader>& result = cache.vertexShaders[std::string(path)];
    if (!result)
        result = gpu->LoadVertexShader(path, FileGetContent(path));

    return result;
}

std::shared_ptr<PixelShader> Renderer::LoadPixelShader(std::string_view path)
{
    std::shared_ptr<PixelShader>& result = cache.pixelShaders[std::string(path)];
    if (!result)
        result = gpu->LoadPixelShader(path, FileGetContent(path));

    return result;
}

std::shared_ptr<Material> Renderer::CreateMaterial(std::string_view vertexShaderPath, std::string_view pixelShaderPath)
{
    return std::make_shared<Material>(gpu, LoadVertexShader(vertexShaderPath), LoadPixelShader(pixelShaderPath));
}

void Renderer::DrawScene(SceneNode* root)
{
    using namespace DirectX;
    XMMATRIX parent = XMMatrixIdentity();

    DrawScene(root, parent);
}

void Renderer::DrawScene(SceneNode* root, DirectX::FXMMATRIX parent)
{
    using namespace DirectX;

    for (auto& node : root->children) {
        XMMATRIX worldMatrix = node->GetWorldMatrix() * parent;

        if (DXP::RenderObject* obj = dynamic_cast<DXP::RenderObject*>(node.get()); obj) {
            BindMaterial(obj->material.get());
            UpdateConstantBuffers(obj, worldMatrix);
            Draw(obj->material.get(), obj->mesh.get());
        }

        DrawScene(node.get(), worldMatrix);
    }
}

void Renderer::BindMaterial(DXP::Material *material)
{
    int slots = material->constantBuffers.size();

    ConstantBuffer** ptrs = reinterpret_cast<ConstantBuffer **>(alloca(sizeof(void*) * slots));
    memset(ptrs, '\0', sizeof(void*) * slots);

    ptrs[ConstantBufferSlot::CB_Transform] = transformConstantBuffer.get();
    for (int i = ConstantBufferSlot::CB_Material; i < slots; i++) {
        ptrs[i] = material->constantBuffers[i].get();
    }

    if (material->vertexShader.program)
        gpu->BindVertexConstantBuffers(ptrs, slots, 0);

    if (material->pixelShader.program)
        gpu->BindPixelConstantBuffers(ptrs, slots, 0);

    gpu->BindVertexShader(material->vertexShader.program.get());
    gpu->BindPixelShader(material->pixelShader.program.get());
}

void Renderer::UpdateConstantBuffers(DXP::RenderObject* object, DirectX::FXMMATRIX worldMatrix)
{
    using namespace DirectX;

    // Transform CB
    {
        ConstantBuffer* cb = transformConstantBuffer.get();
        XMFLOAT4X4 matrix;

        XMStoreFloat4x4(&matrix, XMMatrixTranspose(worldMatrix));
        gpu->UpdateConstantBuffer(cb, &matrix, sizeof(matrix));
    }

    if (object->material->constantBufferPerMaterial) {
        ConstantBuffer* cb = object->material->constantBuffers[ConstantBufferSlot::CB_Material].get();
        gpu->UpdateConstantBuffer(cb, object->material->constantBufferPerMaterial->buffer.data(), object->material->constantBufferPerMaterial->buffer.size());
    }

    if (object->constantBufferPerObject) {
        ConstantBuffer* cb = object->material->constantBuffers[ConstantBufferSlot::CB_Object].get();       
        gpu->UpdateConstantBuffer(cb, object->constantBufferPerObject->buffer.data(), object->constantBufferPerObject->buffer.size());
    }
}

void Renderer::Draw(Material *material, Mesh* mesh)
{
    gpu->BindTopology(mesh->topology);

    if (mesh->indices && !mesh->indexBuffer)
        mesh->indexBuffer = gpu->LoadIndexBuffer(mesh->indices.get());

    int maxSlots = gpu->GetLimitValue(RenderBackend::Limit::VertexBufferSlots);
    const VertexBuffer** vertexBuffers = reinterpret_cast<const VertexBuffer **>(alloca(sizeof(void *) * maxSlots));
    memset(vertexBuffers, '\0', maxSlots * sizeof(void*));

    VertexShaderInputLayout inputLayout;
    int i = 0;

    const auto& shaderInput = material->vertexShader.program->GetInputFormat();
    for (VertexShaderInput input : shaderInput) {
        int index = static_cast<int>(input);
        DXP_ASSERT(mesh->channels[index], "Unable to find matching channel {} in mesh", index);

        if (!mesh->vertexBuffers[index])
            mesh->vertexBuffers[index] = gpu->LoadVertexBuffer(mesh->channels[index].get());
        
        vertexBuffers[i++] = mesh->vertexBuffers[index].get();
        inputLayout.PushElement(input, mesh->channels[index]->Format());
    }

    gpu->BindVertexShaderInputLayout(material->vertexShader.program.get(), inputLayout);
    gpu->BindVertexBuffers(&vertexBuffers[0], i, 0);

    if (mesh->indexBuffer) {
        gpu->BindIndexBuffer(mesh->indexBuffer.get()); 
        gpu->DrawIndexed(mesh->indices->Elements());
    } else {
        gpu->Draw(mesh->channels[0]->Elements());
    }
}

};
