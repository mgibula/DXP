#pragma once

namespace DXP
{

struct Engine;
struct VertexShader;
struct PixelShader;

struct VertexBuffer
{
    virtual ~VertexBuffer() = default;
};

struct IndexBuffer
{
    virtual ~IndexBuffer() = default;
};

struct ConstantBuffer
{
    virtual ~ConstantBuffer() = default;

    virtual size_t GetSize() = 0;
};

struct Sampler
{
    virtual ~Sampler() = default;
};

struct Rasterizer
{
    virtual ~Rasterizer() = default;
};

enum class Topology {
    Triangles,
    LineList,
    LineStrip,
};

struct RenderBackend
{
    enum class Limit {
        ConstantBufferSlots,
        VertexBufferSlots,
    };

    virtual ~RenderBackend() = default;

    virtual std::string InfoString() const = 0;

    virtual bool Initialize() = 0;
    virtual void PreRenderLoop(Engine* engine) { };
    virtual void PostRenderLoop(Engine* engine) { };
    virtual void OnFrameStart(Engine* engine) { };
    virtual void OnFrameEnd(Engine* engine) { };

    virtual void ImGuiInit() { };
    virtual void ImGuiShutdown() { };
    virtual void ImGuiFrameStart() { };
    virtual void ImGuiFrameEnd() { };

    virtual uint64_t GetLimitValue(Limit limit) = 0;

    virtual void Display() = 0;

    virtual std::shared_ptr<VertexShader> LoadVertexShader(std::string_view path, std::string_view content) = 0;
    virtual std::shared_ptr<PixelShader> LoadPixelShader(std::string_view path, std::string_view content) = 0;

    virtual void BindVertexShader(VertexShader* shader) = 0;
    virtual void BindPixelShader(PixelShader* shader) = 0;
    virtual void BindTopology(Topology topology) = 0;

    virtual std::shared_ptr<VertexBuffer> LoadVertexBuffer(const BufferBase* buffer) = 0;
    virtual std::shared_ptr<IndexBuffer> LoadIndexBuffer(const BufferBase* buffer) = 0;

    virtual std::shared_ptr<ConstantBuffer> CreateConstantBuffer(size_t size) = 0;
    virtual void UpdateConstantBuffer(ConstantBuffer* buffer, const void* data, size_t size) = 0;

    virtual void BindVertexConstantBuffers(ConstantBuffer** buffers, int count, int startingSlot) = 0;
    virtual void BindPixelConstantBuffers(ConstantBuffer** buffers, int count, int startingSlot) = 0;

    virtual std::shared_ptr<Sampler> CreateSampler(const SamplerSettings& settings) = 0;
    virtual void BindSamplers(const Sampler** samplers, int count, int startingSlot) = 0;

    virtual std::shared_ptr<Rasterizer> CreateRasterizer(const RasterizerSettings& settings) = 0;
    virtual void BindRasterizer(const Rasterizer* rasterizer) = 0;

    virtual std::shared_ptr<RenderTexture> CreateRenderTexture(int width, int height) = 0;
    virtual std::shared_ptr<RenderTarget> GetScreenRenderTarget() = 0;

    virtual void BindRenderTarget(const RenderTarget* target, DepthStencilTexture *depthBuffer) = 0;
    virtual void ClearRenderTarget(RenderTarget* target) = 0;
    virtual void ResizeRenderTarget(RenderTarget* target, int width, int height) = 0;

    virtual std::shared_ptr<DepthStencilTexture> CreateDepthStencilTexture(int width, int height) = 0;
    virtual void ClearDepthStencilTexture(DepthStencilTexture* texture, bool clearDepth, bool clearStencil) = 0;

    virtual std::shared_ptr<DepthStencilTest> CreateDepthStencilTest(bool depthEnabled) = 0;
    virtual void BindDepthStencilTest(DepthStencilTest* test) = 0;

    virtual std::shared_ptr<Viewport> CreateViewport(int x, int y, int width, int height) = 0;
    virtual void BindViewport(const Viewport *viewport) = 0;

    virtual std::shared_ptr<Texture> CreateTexture2D(const TextureData& textureData) = 0;
    virtual void BindTextures(const Texture** textures, int count, int startingSlot) = 0;

    virtual void BindVertexShaderInputLayout(VertexShader* shader, const VertexShaderInputLayout& layout) = 0;
    virtual void BindVertexBuffers(const VertexBuffer** buffers, int count, int startingSlot) = 0;
    virtual void BindIndexBuffer(const IndexBuffer* buffer) = 0;

    virtual void Draw(uint32_t count) = 0;
    virtual void DrawIndexed(uint32_t count) = 0;
};

};
