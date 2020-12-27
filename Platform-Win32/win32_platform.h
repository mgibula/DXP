#include "../Engine/platform.h"
#include <windows.h>
#include <string>

namespace DXP {

struct Win32Platform final : public Platform
{
    static std::string GetLastSystemError();

    Win32Platform(HWND window);

    virtual void* AllocateRawMemory(uint64_t size) override;
    virtual void FreeRawMemory(void* ptr) override;

    virtual int64_t GetTickFrequency() const override;
    virtual int64_t GetTickCounter() const override; 
    virtual void SleepMilliseconds(int32_t milliseconds) override;

    virtual void PreRenderLoop() override;
    virtual void PostRenderLoop() override;
    virtual void OnFrameStart() override;

    virtual std::vector<RenderBackendDescription> GetAvailableRenderers() const override;
    virtual std::unique_ptr<RenderBackend> CreateRenderBackend(std::string_view name) override;

private:
    HWND window;
    std::vector<RenderBackendDescription> renderers;
};

};
