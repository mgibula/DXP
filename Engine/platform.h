#pragma once
#include <cstdint>

namespace DXP {

struct Platform
{
    virtual ~Platform() = default;

    virtual void* AllocateRawMemory(uint64_t size) = 0;
    virtual void FreeRawMemory(void* ptr) = 0;

    virtual int64_t GetTickFrequency() const = 0;
    virtual int64_t GetTickCounter() const = 0;
    virtual void SleepMilliseconds(int32_t milliseconds) = 0;

    virtual void PreRenderLoop() = 0;
    virtual void PostRenderLoop() = 0;
    virtual void OnFrameStart() = 0;
};

}
