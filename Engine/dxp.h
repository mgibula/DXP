#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/sink.h"
#include "spdlog/sinks/base_sink.h"

// Prefix Engine/ is used so this file works in other projects too
#include "Engine/cyclic_buffer.h"
#include "Engine/cyclic_log.h"

#include "Engine/utils.h"
#include "Engine/event.h"
#include "Engine/shader.h"
#include "Engine/render_backend.h"
#include "Engine/renderer.h"
#include "Engine/layer.h"
#include "Engine/simulation.h"
#include "Engine/engine.h"
