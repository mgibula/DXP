#include "pch.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/msvc_sink.h"
#include "../Engine/utils.h"
#include "../Engine/engine.h"
#include "win32_platform.h"

using Win32 = DXP::Win32Platform;

static LRESULT CALLBACK Win32MessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    switch (message) {
        case WM_KEYDOWN: {
            if (!((lParam >> 30) & 1)) {
                uint8_t keycode = (uint8_t)wParam;
                //if (keycode == ' ')
                //    engine->TogglePaused();
            }
            break;
        }
        case WM_KEYUP: {
            uint8_t keycode = (uint8_t)wParam;
            //input.keyboard.pressed[keycode] = false;
            //input.add_event(keycode, false);
            break;
        }
        case WM_MOUSEWHEEL: {
            //engine->SubmitEvent(DXP::Event::Type::MouseScrolled, GET_WHEEL_DELTA_WPARAM(wParam));
            //input.mouse_delta.wheel = GET_WHEEL_DELTA_WPARAM(wParam);
            //input.mouse.wheel += GET_WHEEL_DELTA_WPARAM(wParam);
            break;
        }
        case WM_LBUTTONUP: {
            //input.mouse_delta.left_button = true;
            //input.mouse.left_button = false;
            break;
        }
        case WM_MBUTTONUP: {
            //input.mouse_delta.middle_button = true;
            //input.mouse.middle_button = false;
            break;
        }
        case WM_RBUTTONUP: {
            //input.mouse_delta.right_button = true;
            //input.mouse.right_button = false;
            break;
        }
        case WM_LBUTTONDOWN: {
            //input.mouse_delta.left_button = true;
            //input.mouse.left_button = true;
            break;
        }
        case WM_MBUTTONDOWN: {
            //input.mouse_delta.middle_button = true;
            //input.mouse.middle_button = true;
            break;
        }
        case WM_RBUTTONDOWN: {
            //input.mouse_delta.right_button = true;
            //input.mouse.right_button = true;
            break;
        }
        case WM_MOUSEMOVE: {
            //input.mouse_delta.x = GET_X_LPARAM(lParam) - input.mouse.x;
            //input.mouse_delta.y = GET_Y_LPARAM(lParam) - input.mouse.y;
            //input.mouse.x = GET_X_LPARAM(lParam);
            //input.mouse.y = GET_Y_LPARAM(lParam);
            break;
        }
        case WM_DESTROY:
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(window, message, wParam, lParam);
}

int WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance, _In_ LPSTR cmdLine, _In_ int showCode)
{
    //_CrtSetAllocHook(AllocationDebugHook);

    auto sink = std::make_shared<spdlog::sinks::msvc_sink_st>();
    auto main_logger = std::make_shared<spdlog::logger>("main", sink);

    spdlog::register_logger(main_logger);
    spdlog::set_default_logger(main_logger);

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
