#include "pch.h"
#include "win32_platform.h"
#include "../Imgui/backends/imgui_impl_win32.h"
#include "../Engine/utils.h"
#include "../Engine/engine.h"
#include "../Engine/event.h"
#include "../Engine/render_backend.h"
#include "../Renderer-DirectX11/directx11_backend.h"
#include <windowsx.h>

#pragma comment(lib, "winmm.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

void Win32Platform::PreRenderLoop(Engine* engine)
{
    // For high resolution sleep
    if (timeBeginPeriod(1) != TIMERR_NOERROR)
        Fatal(FMT_STRING("timeBeginPeriod failed"));
}

void Win32Platform::PostRenderLoop(Engine* engine)
{
}

void Win32Platform::OnFrameStart(Engine* engine)
{
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);

        ImGui_ImplWin32_WndProcHandler(window, msg.message, msg.wParam, msg.lParam);

        switch (msg.message) {
        case WM_QUIT:
            engine->Terminate();
            break;
        case WM_LBUTTONDOWN:
            engine->SubmitEvent(Event::MouseButtonPressed(Event::MouseButton::Left));
            break;
        case WM_MBUTTONDOWN:
            engine->SubmitEvent(Event::MouseButtonPressed(Event::MouseButton::Middle));
            break;
        case WM_RBUTTONDOWN:
            engine->SubmitEvent(Event::MouseButtonPressed(Event::MouseButton::Right));
            break;
        case WM_LBUTTONUP:
            engine->SubmitEvent(Event::MouseButtonReleased(Event::MouseButton::Left));
            break;
        case WM_MBUTTONUP:
            engine->SubmitEvent(Event::MouseButtonReleased(Event::MouseButton::Middle));
            break;
        case WM_RBUTTONUP:
            engine->SubmitEvent(Event::MouseButtonReleased(Event::MouseButton::Right));
            break;
        case WM_MOUSEMOVE:
            engine->SubmitEvent(Event::MouseMoved(GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam)));
            break;
        case WM_MOUSEWHEEL:
            engine->SubmitEvent(Event::MouseScrolled(0, GET_WHEEL_DELTA_WPARAM(msg.wParam)));
            break;
        case WM_MOUSEHWHEEL:
            engine->SubmitEvent(Event::MouseScrolled(GET_WHEEL_DELTA_WPARAM(msg.wParam), 0));
            break;
        case WM_KEYDOWN:
            if (!((msg.lParam >> 30) & 1))
                engine->SubmitEvent(Event::KeyPressed(static_cast<int>(msg.wParam)));
            break;
        case WM_KEYUP:
            engine->SubmitEvent(Event::KeyReleased(static_cast<int>(msg.wParam)));
            break;
        case WM_SIZE:
            if (msg.wParam == SIZE_MINIMIZED) {
                engine->SubmitEvent(Event::ApplicationMinimized());
            }
            else {
                engine->SubmitEvent(Event::ApplicationResized(static_cast<int>(LOWORD(msg.lParam)), static_cast<int>(HIWORD(msg.lParam))));
            }
            break;
        }

        DispatchMessage(&msg);
    }
}

void Win32Platform::OnFrameEnd(Engine* engine)
{
}

void Win32Platform::ImGuiInit()
{
    ImGui_ImplWin32_Init(window);
}

void Win32Platform::ImGuiShutdown()
{
    ImGui_ImplWin32_Shutdown();
}

void Win32Platform::ImGuiFrameStart()
{
    ImGui_ImplWin32_NewFrame();
}

void Win32Platform::ImGuiFrameEnd()
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