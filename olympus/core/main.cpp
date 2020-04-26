#include <iostream>
#include <utils/olyerror.h>
#include <logging/logging.h>
#include <managers/CommandLineManager.h>
#include <Engine.h>

int main(int argc, char** argv)
{
    oly::logging::initialize();
    olyCommandLineManager.initialize(argc, argv);

    olyEngine.initialize();

    return olyEngine.run();
}
