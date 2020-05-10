#include <pch.h>

#include <managers/CommandLineManager.h>
#include <utils/threading_utils.h>

#include <Engine.h>

int main(int argc, char** argv)
{
    oly::logging::initialize();

    oly::threading::defineCurrentThreadAsMain();

    olyCommandLineManager.initialize(argc, argv);

    olyEngine.initialize();

    return olyEngine.run();
}
