#include "../Engine/platform.h"

namespace DXP {

struct Win32Platform final : public Platform
{
    // Ugly but needed for windows queue processing
    static Win32Platform* platform_instance;
    static Engine* engine_instance;

    static std::string GetLastSystemError();

    Win32Platform(HWND window);

    static LRESULT HandleMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(const MSG& msg, Engine* engine);

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
