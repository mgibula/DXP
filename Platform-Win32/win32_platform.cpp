#include "win32_platform.h"
#include "../Imgui/backends/imgui_impl_win32.h"
#include "../Engine/utils.h"
#include "../Engine/render_backend.h"
#include "../Renderer-DirectX11/directx11_backend.h"
#include <fmt/format.h>
#include <Windows.h>
#include <timeapi.h>

#pragma comment(lib, "winmm.lib")

namespace DXP
{

void Fatal(std::string_view message)
{
    MessageBox(nullptr, message.data(), "Fatal error", MB_ICONERROR | MB_OK);
    ExitProcess(1);
}

Win32Platform::Win32Platform(HWND window) :
    window(window)
{
    renderers.push_back({ "DirectX11", "DirectX 11 Renderer" });
}

void* Win32Platform::AllocateRawMemory(uint64_t size)
{
    void* result = VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!result)
        Fatal(FMT_STRING("Allocation of {} bytes failed: {}"), size, GetLastSystemError());

    return result;
}

void Win32Platform::FreeRawMemory(void* ptr)
{
    VirtualFree(ptr, 0, MEM_RELEASE);
}

int64_t Win32Platform::GetTickFrequency() const
{
    LARGE_INTEGER ticksFrequency;
    if (!QueryPerformanceFrequency(&ticksFrequency))
        Fatal(FMT_STRING("QueryPerformanceFrequency failed: {}"), GetLastSystemError());

    return ticksFrequency.QuadPart;
}

int64_t Win32Platform::GetTickCounter() const
{
    LARGE_INTEGER result;
    if (!QueryPerformanceCounter(&result))
        Fatal(FMT_STRING("QueryPerformanceCounter failed: {}"), GetLastSystemError());

    return result.QuadPart;
}

void Win32Platform::SleepMilliseconds(int32_t milliseconds)
{
    Sleep(DWORD(milliseconds));
}

void Win32Platform::PreRenderLoop()
{
    // For high resolution sleep
    if (timeBeginPeriod(1) != TIMERR_NOERROR)
        Fatal(FMT_STRING("timeBeginPeriod failed"));

    ImGui_ImplWin32_Init(window);
}

void Win32Platform::PostRenderLoop()
{
    ImGui_ImplWin32_Shutdown();
}

void Win32Platform::OnFrameStart()
{
    ImGui_ImplWin32_NewFrame();

    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Win32Platform::OnFrameEnd()
{
}

std::vector<RenderBackendDescription> Win32Platform::GetAvailableRenderers() const
{
    return renderers;
}

std::unique_ptr<RenderBackend> Win32Platform::CreateRenderBackend(std::string_view name)
{
    std::unique_ptr<RenderBackend> result;
    if (name == "DirectX11")
        result = std::make_unique<DirectX11Backend>(window);

    if (!result || !result->Initialize())
        return nullptr;

    return result;
}

std::string Win32Platform::GetLastSystemError()
{
    DWORD error = GetLastError();
    char* message;

    uint32_t bytes = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        error,
        0,
        (LPTSTR)&message,
        0, NULL
    );

    if (!bytes)
        return fmt::format("[Unknown system error message for {}]", error);

    std::string result{ message, bytes };
    LocalFree(message);

    return result;
}

}