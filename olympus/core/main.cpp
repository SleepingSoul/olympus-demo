#include <iostream>
#include <extra_std/extra_std.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <easy/profiler.h>
#include <cxxopts.hpp>

int main(int argc, char** argv)
{
    profiler::startListen();

    cxxopts::Options options("olympus", "Augumented reality engine");

    options.add_options()
        ("w,width", "window width in px", cxxopts::value<unsigned>()->default_value("800"))
        ("h,height", "window height in px", cxxopts::value<unsigned>()->default_value("600"));

    auto logger = spdlog::basic_logger_mt("logger works!", "logger works.txt");
    logger->debug(estd::format("ESTD works, {}!", "yay"));

    try
    {
        auto result = options.parse(argc, argv);
        auto value = result["width"].as<unsigned>();
    }
    catch (const cxxopts::OptionException& e)
    {
        logger->error(e.what());
    }

    EASY_FUNCTION(profiler::colors::Magenta);


    return 0;
}
