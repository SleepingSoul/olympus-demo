#include "logging.h"
#include <vector>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace
{
    auto LogFileName = "last_launch_logs.txt";
    auto LoggerName = "olympus_logger";
    const bool FlushOldLogger = true;
}

void oly::logging::initialize()
{
    const std::vector<spdlog::sink_ptr> sinks = {
#ifdef _DEBUG
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
#endif
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(LogFileName, FlushOldLogger)
    };

    auto logger = std::make_shared<spdlog::logger>(LoggerName, sinks.begin(), sinks.end());
    spdlog::set_default_logger(std::move(logger));

#ifdef _DEBUG
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif
}
