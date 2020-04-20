#include <iostream>
#include <extra_std/extra_std.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <easy/profiler.h>
#include <cxxopts.hpp>

int main(int argc, char** argv)
{
    profiler::startListen();

    EASY_FUNCTION(profiler::colors::Magenta);

    auto logger = spdlog::basic_logger_mt("logger works!", "logger works.txt");
    logger->debug(estd::format("ESTD works, {}!", "yay"));

    return 0;
}
