#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <string_view>
#include <fmt/format.h>
#include <chrono>
#include <thread>
#include <optional>

#include "spdlog/spdlog.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/sink.h"
#include "spdlog/sinks/base_sink.h"
#include "spdlog/sinks/ringbuffer_sink.h"

#include "cyclic_buffer.h"
#include "cyclic_log.h"
