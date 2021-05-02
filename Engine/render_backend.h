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

enum class Topology {
    Triangles,
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

    virtual void ClearScreen() = 0;
    virtual void Display() = 0;
    virtual void Resize(int width, int height) = 0;

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

    virtual void BindVertexShaderInputLayout(VertexShader* shader, const VertexShaderInputLayout& layout) = 0;
    virtual void BindVertexBuffers(const VertexBuffer** buffers, int count, int startingSlot) = 0;
    virtual void BindIndexBuffer(const IndexBuffer* buffer) = 0;

    virtual void Draw(uint32_t count) = 0;
    virtual void DrawIndexed(uint32_t count) = 0;
};

};
