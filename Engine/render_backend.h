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

enum class Topology {
    Triangles,
};

struct RenderBackend
{
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

    virtual void BindVertexBuffers(const VertexBuffer** buffers, int count, int startingSlot) = 0;
    virtual void BindIndexBuffer(const IndexBuffer* buffer) = 0;

    virtual void Draw(uint32_t count) = 0;
    virtual void DrawIndexed(uint32_t count) = 0;
};

};
