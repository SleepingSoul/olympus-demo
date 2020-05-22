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

    logging::LogLevel CurrentLogLevel = logging::LogLevel::Debug;

    constexpr spdlog::level::level_enum convertLogLevel(logging::LogLevel logLvl)
    {
        using namespace logging;
        using namespace spdlog;

        switch (logLvl)
        {
        case LogLevel::Debug:
            return level::debug;
        case LogLevel::Info:
            return level::info;
        case LogLevel::Warning:
            return level::warn;
        case LogLevel::Error:
            return level::err;
        case LogLevel::Critical:
        default:
            return level::critical;
        }
    }
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
    setLogLevel(LogLevel::Debug);
#else
    setLogLevel(LogLevel::Debug);
#endif
}

void logging::setLogLevel(LogLevel logLevel)
{
    CurrentLogLevel = logLevel;
    spdlog::set_level(convertLogLevel(logLevel));
}

logging::LogLevel logging::getLogLevel()
{
    return CurrentLogLevel;
}

EndNamespaceOlympus
