#include "win32_platform.h"
#include "../Engine/utils.h"
#include <fmt/format.h>
#include <Windows.h>
#include <timeapi.h>

namespace DXP
{

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

    //ImGui_ImplWin32_Init(window);
}

void Win32Platform::PostRenderLoop()
{
    //ImGui_ImplWin32_Shutdown();
}

void Win32Platform::OnFrameStart()
{
    //ImGui_ImplWin32_NewFrame();

    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

}