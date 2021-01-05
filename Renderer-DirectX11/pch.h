#pragma once

#include <windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <fmt/format.h>
#include <stdio.h>
#include <d3dcompiler.h>
#include <string_view>

#include "spdlog/spdlog.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/sink.h"
#include "spdlog/sinks/base_sink.h"
#include "spdlog/sinks/ringbuffer_sink.h"

#include "../Engine/render_backend.h"
#include "../Engine/cyclic_buffer.h"
#include "../Engine/cyclic_log.h"
#include "../Engine/utils.h"

#include "directx11_backend.h"

#include "../Imgui/backends/imgui_impl_dx11.h"
