#pragma once

namespace DXP
{

struct Engine;
struct VertexShader;
struct PixelShader;

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
};

};
