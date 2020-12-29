#pragma once

namespace DXP {

// This needs to be defined by platform layer
[[noreturn]] void Fatal(std::string_view message);

struct RenderBackendDescription
{
    std::string name;
    std::string description;
};

struct Engine;
struct RenderBackend;

struct Platform
{
    virtual ~Platform() = default;

    virtual void* AllocateRawMemory(uint64_t size) = 0;
    virtual void FreeRawMemory(void* ptr) = 0;

    virtual void PreRenderLoop(Engine* engine) = 0;
    virtual void PostRenderLoop(Engine* engine) = 0;
    virtual void OnFrameStart(Engine* engine) = 0;
    virtual void OnFrameEnd(Engine* engine) = 0;

    virtual std::vector<RenderBackendDescription> GetAvailableRenderers() const = 0;
    virtual std::unique_ptr<RenderBackend> CreateRenderBackend(std::string_view name) = 0;
};

}
