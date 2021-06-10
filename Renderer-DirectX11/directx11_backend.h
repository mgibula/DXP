#pragma once

namespace DXP
{

struct DirectX11Backend final : public RenderBackend
{
    DirectX11Backend(HWND window, std::shared_ptr<spdlog::logger> log);
    ~DirectX11Backend();

    virtual std::string InfoString() const override;

    virtual bool Initialize() override;
    virtual void PreRenderLoop(Engine* engine) override;
    virtual void PostRenderLoop(Engine* engine) override;
    virtual void OnFrameStart(Engine* engine) override;
    virtual void OnFrameEnd(Engine* engine) override;

    virtual void ImGuiInit() override;
    virtual void ImGuiShutdown() override;
    virtual void ImGuiFrameStart() override;
    virtual void ImGuiFrameEnd() override;

    virtual uint64_t GetLimitValue(Limit limit) override;

    virtual void Display() override;

    virtual std::shared_ptr<VertexShader> LoadVertexShader(std::string_view path, std::string_view content) override;
    virtual std::shared_ptr<PixelShader> LoadPixelShader(std::string_view path, std::string_view content) override;

    virtual void BindVertexShader(VertexShader* shader) override;
    virtual void BindPixelShader(PixelShader* shader) override;
    virtual void BindTopology(Topology topology) override;

    virtual std::shared_ptr<VertexBuffer> LoadVertexBuffer(const BufferBase* buffer) override;
    virtual std::shared_ptr<IndexBuffer> LoadIndexBuffer(const BufferBase* buffer) override;

    virtual std::shared_ptr<ConstantBuffer> CreateConstantBuffer(size_t size) override;
    virtual void UpdateConstantBuffer(ConstantBuffer* buffer, const void* data, size_t size) override;

    virtual void BindVertexConstantBuffers(ConstantBuffer** buffers, int count, int startingSlot) override;
    virtual void BindPixelConstantBuffers(ConstantBuffer** buffers, int count, int startingSlot) override;

    virtual std::shared_ptr<Sampler> CreateSampler(const SamplerSettings& settings) override;
    virtual void BindSamplers(Sampler** samplers, int count, int startingSlot) override;

    virtual std::shared_ptr<Rasterizer> CreateRasterizer(const RasterizerSettings& settings) override;
    virtual void BindRasterizer(const Rasterizer* rasterizer) override;

    virtual std::shared_ptr<RenderTexture> CreateRenderTexture(int width, int height) override;
    virtual std::shared_ptr<RenderTarget> GetScreenRenderTarget() override;

    virtual void BindRenderTarget(const RenderTarget* target, DepthStencilTexture* depthBuffer) override;
    virtual void ClearRenderTarget(RenderTarget* target) override;
    virtual void ResizeRenderTarget(RenderTarget* target, int width, int height) override;

    virtual std::shared_ptr<DepthStencilTexture> CreateDepthStencilTexture(int width, int height) override;
    virtual void ClearDepthStencilTexture(DepthStencilTexture* texture, bool clearDepth, bool clearStencil) override;

    virtual std::shared_ptr<DepthStencilTest> CreateDepthStencilTest(bool depthEnabled) override;
    virtual void BindDepthStencilTest(DepthStencilTest* test) override;

    virtual std::shared_ptr<Viewport> CreateViewport(int x, int y, int width, int height) override;
    virtual void BindViewport(const Viewport* viewport) override;

    virtual std::shared_ptr<Texture> CreateTexture2D(const TextureData& textureData) override;
    virtual void BindTextures(const Texture** textures, int count, int startingSlot) override;

    virtual void BindVertexShaderInputLayout(VertexShader* shader, const VertexShaderInputLayout& layout) override;
    virtual void BindVertexBuffers(const VertexBuffer** buffers, int count, int startingSlot) override;
    virtual void BindIndexBuffer(const IndexBuffer* buffer) override;

    virtual void Draw(uint32_t count) override;
    virtual void DrawIndexed(uint32_t count) override;

private:
    HWND window;
    int width = 0;
    int height = 0;
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    std::shared_ptr<spdlog::logger> log;
    std::shared_ptr<DirectX11BackbufferRenderTarget> backbufferRenderTarget;

    std::map<VertexShaderInputLayout, Microsoft::WRL::ComPtr<ID3D11InputLayout>> inputLayouts;
};

};
