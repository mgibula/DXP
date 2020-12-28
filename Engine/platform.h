#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <string_view>
#include <memory>

namespace DXP {

// This needs to be defined by platform layer
[[noreturn]] void Fatal(std::string_view message);

struct RenderBackendDescription
{
    std::string name;
    std::string description;
};

struct RenderBackend;

struct Platform
{
    virtual ~Platform() = default;

    virtual void* AllocateRawMemory(uint64_t size) = 0;
    virtual void FreeRawMemory(void* ptr) = 0;

    virtual void PreRenderLoop() = 0;
    virtual void PostRenderLoop() = 0;
    virtual void OnFrameStart() = 0;
    virtual void OnFrameEnd() = 0;

    virtual std::vector<RenderBackendDescription> GetAvailableRenderers() const = 0;
    virtual std::unique_ptr<RenderBackend> CreateRenderBackend(std::string_view name) = 0;
};

}
