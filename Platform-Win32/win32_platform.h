#include "../Engine/platform.h"

namespace DXP {

struct Win32Platform final : public Platform
{
    static std::string GetLastSystemError();

    Win32Platform(HWND window);

    virtual void* AllocateRawMemory(uint64_t size) override;
    virtual void FreeRawMemory(void* ptr) override;

    virtual void PreRenderLoop() override;
    virtual void PostRenderLoop() override;
    virtual void OnFrameStart() override;
    virtual void OnFrameEnd() override;

    virtual std::vector<RenderBackendDescription> GetAvailableRenderers() const override;
    virtual std::unique_ptr<RenderBackend> CreateRenderBackend(std::string_view name) override;

private:
    HWND window;
    std::vector<RenderBackendDescription> renderers;
};

};
