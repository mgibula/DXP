#pragma once

#define DXP_ASSERT(c, ...)         do { if (!(c)) DXP::Fatal(__VA_ARGS__); } while (0)

namespace DXP
{

[[noreturn]] void Fatal(std::string_view message);

template <typename S, typename... Args>
[[noreturn]] void Fatal(S&& format, Args&&... args)
{
    try {
        Fatal(std::string_view{ fmt::format(format, std::forward<Args>(args)...) });
    }
    catch (const fmt::format_error& e) {
        Fatal(std::string_view{ fmt::format("Error while parsing fatal error format string: {}", e.what()) });
    }
}

};
