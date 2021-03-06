#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/sink.h"
#include "spdlog/sinks/base_sink.h"

#include <DirectXMath.h>
#include <tuple>
#include <functional>

// Prefix Engine/ is used so this file works in other projects too
#include "Engine/cyclic_buffer.h"
#include "Engine/cyclic_log.h"

#include "Engine/utils.h"
#include "Engine/misc/hash-tuple.h"
#include "Engine/event.h"

#include "Engine/codex.h"
#include "Engine/render-target.h"
#include "Engine/resources/rasterizer.h"
#include "Engine/texture.h"
#include "Engine/resources/depth-stencil.h"
#include "Engine/texture-loader.h"
#include "Engine/resources/sampler.h"
#include "Engine/constant-buffer.h"
#include "Engine/buffer.h"
#include "Engine/shader.h"
#include "Engine/render_backend.h"
#include "Engine/mesh.h"
#include "Engine/material.h"

#include "Engine/loader/base.h"
#include "Engine/loader/depth-stencil.h"
#include "Engine/loader/rasterizer.h"
#include "Engine/loader/sampler.h"

#include "Engine/renderer/resources.h"
#include "Engine/renderer/renderer.h"

#include "Engine/render-object.h"
#include "Engine/camera.h"
#include "Engine/layer.h"
#include "Engine/simulation.h"
#include "Engine/engine.h"

#include "Engine/ring_buffer.h"

#include "Engine/layers/input.h"
#include "Engine/layers/debug.h"