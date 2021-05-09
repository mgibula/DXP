#pragma once

namespace DXP
{

struct DirectX11Backend final : public RenderBackend
{
    DirectX11Backend(HWND window, std::shared_ptr<spdlog::logger> log);

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

    virtual void ClearScreen() override;
    virtual void Display() override;
    virtual void Resize(int width, int height) override;
    virtual int Width() override;
    virtual int Height() override;

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
    virtual void BindSamplers(const Sampler** samplers, int count, int startingSlot) override;

    virtual std::shared_ptr<Rasterizer> CreateRasterizer(const RasterizerSettings& settings) override;
    virtual void BindRasterizer(const Rasterizer* rasterizer) override;

    virtual std::shared_ptr<RenderTexture> CreateRenderTexture(int width, int height) override;
    virtual void BindRenderTarget(const RenderTexture* target) override;
    virtual void ClearRenderTarget(RenderTexture* target) override;

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
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backbuffer;
    std::shared_ptr<spdlog::logger> log;

    std::map<VertexShaderInputLayout, Microsoft::WRL::ComPtr<ID3D11InputLayout>> inputLayouts;
};

};
