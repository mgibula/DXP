#include "../Engine/platform.h"
#include <windows.h>

namespace DXP {

struct Win32Platform final : public Platform
{
    virtual void* AllocateRawMemory(uint64_t size) override;
    virtual void FreeRawMemory(void* ptr) override;

    virtual int64_t GetTickFrequency() const override;
    virtual int64_t GetTickCounter() const override; 
    virtual void SleepMilliseconds(int32_t milliseconds) override;

    virtual void PreRenderLoop() override;
    virtual void PostRenderLoop() override;
    virtual void OnFrameStart() override;
};

};
