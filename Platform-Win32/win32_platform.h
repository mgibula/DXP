#include "../Engine/platform.h"

namespace DXP {

struct Win32Platform final : public Platform
{
    static std::string GetLastSystemError();

    Win32Platform(HWND window);

    virtual void Initialize(Engine* engine) override;

    virtual void* AllocateRawMemory(uint64_t size) override;
    virtual void FreeRawMemory(void* ptr) override;

    virtual void PreRenderLoop(Engine* engine) override;
    virtual void PostRenderLoop(Engine* engine) override;
    virtual void OnFrameStart(Engine* engine) override;
    virtual void OnFrameEnd(Engine* engine) override;

    virtual void ImGuiInit() override;
    virtual void ImGuiShutdown() override;
    virtual void ImGuiFrameStart() override;
    virtual void ImGuiFrameEnd() override;

    virtual std::vector<RenderBackendDescription> GetAvailableRenderers() const override;
    virtual std::unique_ptr<RenderBackend> CreateRenderBackend(std::string_view name) override;

private:
    HWND window;
    std::vector<RenderBackendDescription> renderers;
    std::shared_ptr<spdlog::logger> log;
};

};
