#include "logging.h"
#include <vector>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

BeginNamespaceOlympus

namespace
{
    const char* const LogFileName = "last_launch_logs.txt";
    const char* const LoggerName = "olympus_logger";
    const char* const LoggerPattern = "[%H:%M:%S.%f] %^[%l]%$ [thread %t] %v";
    const bool FlushOldLogger = true;
}

void logging::initialize()
{
    const std::vector<spdlog::sink_ptr> sinks = {
#ifdef _DEBUG
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
#endif
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(LogFileName, FlushOldLogger)
    };

    auto logger = std::make_shared<spdlog::logger>(LoggerName, sinks.begin(), sinks.end());
    spdlog::set_default_logger(std::move(logger));

    spdlog::flush_on(spdlog::level::level_enum::debug);
    spdlog::set_pattern(LoggerPattern);
#ifdef _DEBUG
    
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::debug);
#endif
}

EndNamespaceOlympus
