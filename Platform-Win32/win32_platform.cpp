#include "pch.h"

#pragma comment(lib, "winmm.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace DXP
{

Win32Platform* Win32Platform::platform_instance;
Engine* Win32Platform::engine_instance;

void Fatal(std::string_view message)
{
    MessageBox(nullptr, message.data(), "Fatal error", MB_ICONERROR | MB_OK);
    __debugbreak();
    ExitProcess(1);
}

Win32Platform::Win32Platform(HWND window) :
    window(window)
{
    RECT rect;
    if (!GetClientRect(window, &rect))
        Fatal("GetClientRect");

    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    Win32Platform::platform_instance = this;
    renderers.push_back({ "DirectX11", "DirectX 11" });
}

void Win32Platform::Initialize(Engine* engine)
{
    Win32Platform::engine_instance = engine;

    auto msvc_sink = std::make_shared<spdlog::sinks::msvc_sink_st>();
    msvc_sink->set_level(spdlog::level::info);

    engine->AddLogSink(msvc_sink);
    log = engine->CreateLogger("win32");
}

void* Win32Platform::AllocateRawMemory(uint64_t size)
{
    void* result = VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!result)
        Fatal(FMT_STRING("Allocation of {} bytes failed: {}"), size, GetLastSystemError());

    SPDLOG_LOGGER_DEBUG(log, "Allocating {} bytes of memory", size);
    return result;
}

void Win32Platform::FreeRawMemory(void* ptr)
{
    VirtualFree(ptr, 0, MEM_RELEASE);
}

int Win32Platform::ScreenWidth() const
{
    return width;
}

int Win32Platform::ScreenHeight() const
{
    return height;
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

LPARAM Win32Platform::HandleMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (!Win32Platform::platform_instance)
        return DefWindowProc(window, message, wParam, lParam);

    MSG msg = {};
    msg.hwnd = window;
    msg.message = message;
    msg.wParam = wParam;
    msg.lParam = lParam;

    return Win32Platform::platform_instance->HandleMessage(msg, Win32Platform::engine_instance);
}

LPARAM Win32Platform::HandleMessage(const MSG& msg, Engine* engine)
{
    if (ImGui_ImplWin32_WndProcHandler(msg.hwnd, msg.message, msg.wParam, msg.lParam))
        return true;

    switch (msg.message) {
    case WM_DESTROY:
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
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
        } else {
            width = static_cast<int>(LOWORD(msg.lParam));
            height = static_cast<int>(HIWORD(msg.lParam));
            engine->SubmitEvent(Event::ApplicationResized(width, height));
        }
        break;
    }

    return DefWindowProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
}

void Win32Platform::OnFrameStart(Engine* engine)
{
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);

        switch (msg.message) {
        case WM_QUIT:
            engine->Terminate();
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
        result = std::make_unique<DirectX11Backend>(window, log->clone("gpu"));

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