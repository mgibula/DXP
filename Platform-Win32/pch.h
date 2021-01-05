#pragma once

#include <windows.h>
#include <string>
#include <fmt/format.h>
#include <timeapi.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <windowsx.h>

#include "spdlog/spdlog.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/sink.h"
#include "spdlog/sinks/msvc_sink.h"

#include "win32_platform.h"

#include "../Engine/utils.h"
#include "../Engine/event.h"
#include "../Engine/render_backend.h"
#include "../Engine/cyclic_buffer.h"
#include "../Engine/cyclic_log.h"
#include "../Engine/engine.h"

#include "../Imgui/backends/imgui_impl_win32.h"
#include "../Renderer-DirectX11/directx11_backend.h"
