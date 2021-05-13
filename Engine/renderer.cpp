#include "pch.h"

namespace DXP
{

Renderer::Renderer(Platform* platform, RenderBackend* gpu, std::shared_ptr<spdlog::logger> log) :
    scene(std::make_unique<SceneRoot>()),
    platform(platform),
    gpu(gpu),
    log(log)
{
    SPDLOG_LOGGER_INFO(log, "Initializing");
    SPDLOG_LOGGER_INFO(log, "Using renderer backend: {}", gpu->InfoString());
    SetRenderBackend(gpu);
}

void Renderer::SetRenderBackend(RenderBackend* backend)
{
    using namespace DirectX;

    transformConstantBuffer = gpu->CreateConstantBuffer(sizeof(XMFLOAT4X4));
    cameraConstantBuffer = gpu->CreateConstantBuffer(sizeof(XMFLOAT4X4) * 2);

    // Create general samplers and bind them
    {
        SamplerSettings settings;
        settings.type = SamplerType::Point;

        pointSampler = gpu->CreateSampler(settings);
    }

    {
        SamplerSettings settings;
        settings.type = SamplerType::Point;
        settings.point.minification_linear = true;
        settings.point.magnification_linear = true;

        bilinearSampler = gpu->CreateSampler(settings);
    }

    {
        SamplerSettings settings;
        settings.type = SamplerType::Point;
        settings.point.minification_linear = true;
        settings.point.magnification_linear = true;
        settings.point.mip_linear = true;

        trilinearSampler = gpu->CreateSampler(settings);
    }

    {
        SamplerSettings settings;
        settings.type = SamplerType::Anisotropic;
        settings.anisotropic.level = 16;

        anisotropicSampler = gpu->CreateSampler(settings);
    }

    std::array<const Sampler*, 4> ptrs;
    ptrs[SamplerSlot::Sampler_Point] = pointSampler.get();
    ptrs[SamplerSlot::Sampler_Bilinear] = bilinearSampler.get();
    ptrs[SamplerSlot::Sampler_Trilinear] = trilinearSampler.get();
    ptrs[SamplerSlot::Sampler_Anisotropic] = anisotropicSampler.get();

    gpu->BindSamplers(ptrs.data(), 4, 0);

    rasterizers.resize(2);
    {
        RasterizerSettings settings;
        settings.wireframe = false;
        settings.drawFront = true;

        rasterizers[Rasterizer_Solid] = gpu->CreateRasterizer(settings);
    }

    {
        RasterizerSettings settings;
        settings.wireframe = true;
        settings.drawFront = true;
        settings.drawBack = true;

        rasterizers[Rasterizer_Wireframe] = gpu->CreateRasterizer(settings);
    }

    depthStencil.resize(2);
    depthStencil[(int)DepthStencil::Disabled] = gpu->CreateDepthStencilTest(false);
    depthStencil[(int)DepthStencil::Enabled] = gpu->CreateDepthStencilTest(true);

    scene->renderTarget = gpu->GetScreenRenderTarget();
    scene->depthStencilTest = DepthStencil::Disabled;
    scene->depthStencilTexture = gpu->CreateDepthStencilTexture(platform->ScreenWidth(), platform->ScreenHeight());
}

void Renderer::OnScreenResize(int width, int height)
{
    gpu->ResizeRenderTarget(gpu->GetScreenRenderTarget().get(), width, height);
    scene->depthStencilTexture = gpu->CreateDepthStencilTexture(width, height);
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

std::shared_ptr<Material> Renderer::CreateMaterial(std::string_view vertexShaderPath, std::string_view pixelShaderPath, std::vector<std::shared_ptr<Texture>> textures)
{
    return std::make_shared<Material>(gpu, LoadVertexShader(vertexShaderPath), LoadPixelShader(pixelShaderPath), std::move(textures));
}

std::shared_ptr<Texture> Renderer::LoadTexture(const TextureData& textureData)
{
    return gpu->CreateTexture2D(textureData);
}

void Renderer::DrawScene(SceneRoot* root)
{
    using namespace DirectX;
    XMMATRIX parent = XMMatrixIdentity();//  root->GetWorldMatrix();
    
    gpu->BindDepthStencilTest(depthStencil[int(root->depthStencilTest)].get());
    gpu->BindRenderTarget(root->renderTarget.get(), root->depthStencilTexture.get());

    gpu->ClearDepthStencilTexture(root->depthStencilTexture.get(), true, true);
    gpu->ClearRenderTarget(root->renderTarget.get());

    if (root->mainCamera) {
        ConstantBuffer* cb = cameraConstantBuffer.get();
        XMFLOAT4X4 matrix[2];

        XMStoreFloat4x4(&matrix[0], XMMatrixTranspose(root->mainCamera->GetViewMatrix()));
        XMStoreFloat4x4(&matrix[1], XMMatrixTranspose(root->mainCamera->GetProjectionMatrix(platform->ScreenWidth(), platform->ScreenHeight()))) ;
        gpu->UpdateConstantBuffer(cb, &matrix[0], sizeof(matrix[0]) * 2);
    }

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
    // Bind shaders
    if (material->vertexShader.program)
        gpu->BindVertexShader(material->vertexShader.program.get());

    if (material->pixelShader.program)
        gpu->BindPixelShader(material->pixelShader.program.get());

    // Bind textures
    {
        int textures = material->textures.size();
        const Texture** ptrs = reinterpret_cast<const Texture**>(_alloca(sizeof(void*) * textures));
        memset(ptrs, '\0', sizeof(void*) * textures);

        for (int i = 0; i < textures; i++) {
            ptrs[i] = material->textures[i].get();
        }

        gpu->BindTextures(ptrs, textures, 0);
    }

    // Bind constant buffers
    {
        int slots = material->constantBuffers.size();
        ConstantBuffer** ptrs = reinterpret_cast<ConstantBuffer**>(_alloca(sizeof(void*) * slots));
        memset(ptrs, '\0', sizeof(void*) * slots);

        ptrs[ConstantBufferSlot::CB_Transform] = transformConstantBuffer.get();
        ptrs[ConstantBufferSlot::CB_Camera] = cameraConstantBuffer.get();
        for (int i = ConstantBufferSlot::CB_Material; i < slots; i++) {
            ptrs[i] = material->constantBuffers[i].get();
        }

        if (material->vertexShader.program)
            gpu->BindVertexConstantBuffers(ptrs, slots, 0);

        if (material->pixelShader.program)
            gpu->BindPixelConstantBuffers(ptrs, slots, 0);
    }
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
    const VertexBuffer** vertexBuffers = reinterpret_cast<const VertexBuffer **>(_alloca(sizeof(void *) * maxSlots));
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
    gpu->BindRasterizer(rasterizers[mesh->rasterizer].get());

    if (mesh->indexBuffer) {
        gpu->BindIndexBuffer(mesh->indexBuffer.get()); 
        gpu->DrawIndexed(mesh->indices->Elements());
    } else {
        gpu->Draw(mesh->channels[0]->GetBufferSize() / mesh->channels[0]->Stride());
    }
}

};
