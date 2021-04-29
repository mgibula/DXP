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
    std::shared_ptr<Material> result = std::make_shared<Material>(LoadVertexShader(vertexShaderPath), LoadPixelShader(pixelShaderPath));

    for (auto slot : { ConstantBufferSlot::Object, ConstantBufferSlot::Material, ConstantBufferSlot::Frame, ConstantBufferSlot::Camera }) {
        const ConstantBufferLayout* layout = result->vertexShader->GetConstantBufferLayout(slot);
        if (layout) {
            SPDLOG_LOGGER_INFO(log, "Creating constant buffer of {} bytes", layout->size);
            result->constantBuffers[static_cast<int>(slot)] = gpu->CreateConstantBuffer(layout->size);
        }
    }

    return result;
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
            BindConstantBuffers(obj, worldMatrix);
            Draw(obj->material.get(), obj->mesh.get());
        }

        DrawScene(node.get(), worldMatrix);
    }
}

void Renderer::BindConstantBuffers(DXP::RenderObject* object, DirectX::FXMMATRIX worldMatrix)
{
    using namespace DirectX;

    {
        ConstantBuffer* cb = transformConstantBuffer.get();
        XMFLOAT4X4 matrix;

        XMStoreFloat4x4(&matrix, XMMatrixTranspose(worldMatrix));
        gpu->UpdateConstantBuffer(cb, &matrix, sizeof(matrix));
        gpu->BindVertexConstantBuffers(&cb, 1, static_cast<int>(ConstantBufferSlot::Transform));
    }

    if (object->material->constantBufferPerMaterial.buffer.size()) {
        int slot = static_cast<int>(ConstantBufferSlot::Material);
        ConstantBuffer* cb = object->material->constantBuffers[slot].get();

        gpu->UpdateConstantBuffer(cb, object->material->constantBufferPerMaterial.buffer.data(), object->material->constantBufferPerMaterial.buffer.size());
        gpu->BindVertexConstantBuffers(&cb, 1, slot);
    }

    if (object->constantBufferPerObject.buffer.size()) {
        int slot = static_cast<int>(ConstantBufferSlot::Object);
        ConstantBuffer* cb = object->material->constantBuffers[slot].get();
        
        gpu->UpdateConstantBuffer(cb, object->constantBufferPerObject.buffer.data(), object->constantBufferPerObject.buffer.size());
        gpu->BindVertexConstantBuffers(&cb, 1, slot);
    }
}

void Renderer::Draw(Material *material, Mesh* mesh)
{
    gpu->BindVertexShader(material->vertexShader.get());
    gpu->BindPixelShader(material->pixelShader.get());
    gpu->BindTopology(mesh->topology);

    if (mesh->indices && !mesh->indexBuffer)
        mesh->indexBuffer = gpu->LoadIndexBuffer(mesh->indices.get());

    std::array<const VertexBuffer*, 16> vertexBuffers;
    VertexShaderInputLayout inputLayout;
    int i = 0;

    const auto& shaderInput = material->vertexShader->GetInputFormat();
    for (VertexShaderInput input : shaderInput) {
        int index = static_cast<int>(input);
        DXP_ASSERT(mesh->channels[index], "Unable to find matching channel {} in mesh", index);

        if (!mesh->vertexBuffers[index])
            mesh->vertexBuffers[index] = gpu->LoadVertexBuffer(mesh->channels[index].get());
        
        vertexBuffers[i++] = mesh->vertexBuffers[index].get();
        inputLayout.PushElement(input, mesh->channels[index]->Format());
    }

    gpu->BindVertexShaderInputLayout(material->vertexShader.get(), inputLayout);
    gpu->BindVertexBuffers(&vertexBuffers[0], i, 0);

    if (mesh->indexBuffer) {
        gpu->BindIndexBuffer(mesh->indexBuffer.get()); 
        gpu->DrawIndexed(mesh->indices->Elements());
    } else {
        gpu->Draw(mesh->channels[0]->Elements());
    }
}

};
