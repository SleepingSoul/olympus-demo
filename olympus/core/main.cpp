#include <iostream>
#include <utils/olyerror.h>
#include <logging/logging.h>
#include <managers/CommandLineManager.h>
#include <easy/profiler.h>

int main(int argc, char** argv)
{
    profiler::startListen();

    oly::logging::initialize();
    olyCommandLineManager.initialize(argc, argv);

    EASY_FUNCTION(profiler::colors::Magenta);

    const auto width = olyCommandLineManager.getLong("--width");

    return 0;
}
