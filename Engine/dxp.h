#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/sink.h"
#include "spdlog/sinks/base_sink.h"

#include <DirectXMath.h>

// Prefix Engine/ is used so this file works in other projects too
#include "Engine/cyclic_buffer.h"
#include "Engine/cyclic_log.h"

#include "Engine/utils.h"
#include "Engine/event.h"

#include "Engine/rasterizer.h"
#include "Engine/texture-loader.h"
#include "Engine/sampler.h"
#include "Engine/texture.h"
#include "Engine/constant-buffer.h"
#include "Engine/buffer.h"
#include "Engine/shader.h"
#include "Engine/render_backend.h"
#include "Engine/mesh.h"
#include "Engine/material.h"
#include "Engine/renderer.h"
#include "Engine/render-object.h"
#include "Engine/camera.h"
#include "Engine/layer.h"
#include "Engine/simulation.h"
#include "Engine/engine.h"

#include "Engine/ring_buffer.h"

#include "Engine/layers/input.h"
#include "Engine/layers/imgui.h"