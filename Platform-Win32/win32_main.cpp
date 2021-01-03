#include "pch.h"
#include "spdlog/spdlog.h"
#include "../Engine/utils.h"
#include "../Engine/engine.h"
#include "win32_platform.h"

using Win32 = DXP::Win32Platform;

static LRESULT CALLBACK Win32MessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    return Win32::HandleMessage(window, message, wParam, lParam);
}

int WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance, _In_ LPSTR cmdLine, _In_ int showCode)
{
    //_CrtSetAllocHook(AllocationDebugHook);

    // Register window class
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = Win32MessageHandler;
    wc.hInstance = instance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wc.lpszClassName = "DXP4_Window";
    wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

    if (!RegisterClassEx(&wc))
        DXP::Fatal(FMT_STRING("RegisterClassEx failed: {}"), Win32::GetLastSystemError());

    // Create and show window
    HWND window = CreateWindowEx(
        NULL,
        wc.lpszClassName,
        "Window Title",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        800,
        600,
        NULL,
        NULL,
        instance,
        NULL
    );

    if (!window)
        DXP::Fatal(FMT_STRING("CreateWindowEx failed: {}"), Win32::GetLastSystemError());

    DXP::Win32Platform platform{ window };
    DXP::Engine engine{ &platform };

    ShowWindow(window, showCode);
    if (!UpdateWindow(window))
        DXP::Fatal(FMT_STRING("UpdateWindow failed: {}"), Win32::GetLastSystemError());

    engine.Run();

    //SG3D::SG3D_Game sg3d_game;

    //GE::Win32Platform platform(window);
    //GE::Win32Directx11 directx11(window);
    //GE::Engine engine(&sg3d_game, &directx11, &platform);

    //engine.Run();

    DestroyWindow(window);

    //// Disable allocation hook, because it will fire when freeing memory that was allocated before WinMain 
    //// and that would cause assert in SubtrackAllocation to fire
    //_CrtSetAllocHook(nullptr);

    return 0;
}
