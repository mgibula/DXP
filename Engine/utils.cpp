#include "utils.h"
#include <cstdint>
#include <Windows.h>

namespace DXP
{

void Fatal(std::string_view message)
{
    MessageBox(nullptr, message.data(), "Fatal error", MB_ICONERROR | MB_OK);
    ExitProcess(1);
}

std::string GetLastSystemError()
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
        (LPTSTR) &message,
        0, NULL
    );

    if (!bytes)
        return fmt::format("[Unknown system error message for {}]", error);

    std::string result{ message, bytes };
    LocalFree(message);

    return result;
}

};
