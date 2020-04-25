#include "CommandLineManager.h"
#include <map>
#include <extra_std/extra_std.h>
#include <logging/logging.h>
#include <utils/olyerror.h>

namespace
{
    const bool ShowHelp = true;

    auto Usage =
        R"(olympus
Usage:
    core.exe [--width=w --height=h]
Options:
    -h, --help            Show this screen.
    --width=<px>          Screen width in pixels [default: 800].
    --height=<px>         Screen height in pixels [default: 600].
        )";
}

oly::CommandLineManager& oly::CommandLineManager::instance()
{
    static oly::CommandLineManager cmdManager{};
    return cmdManager;
}

void oly::CommandLineManager::initialize(int argc, char** argv)
{
    auto args = docopt::docopt(Usage, { std::next(argv), std::next(argv, argc) }, ShowHelp, "debug version");

    m_args.insert(std::make_move_iterator(args.begin()), std::make_move_iterator(args.end()));
}

std::optional<long> oly::CommandLineManager::getLong(std::string_view name) const noexcept(true)
{
    const auto [found, it] = estd::find(m_args, name);

    try
    {
        return found ? std::optional{ it->second.asLong() } : std::nullopt;
    }
    catch (const std::runtime_error&)
    {
        return std::nullopt;
    }
}

std::optional<bool> oly::CommandLineManager::getBool(std::string_view name) const noexcept(true)
{
    const auto [found, it] = estd::find(m_args, name);

    try
    {
        return found ? std::optional{ it->second.asBool() } : std::nullopt;
    }
    catch (const std::runtime_error&)
    {
        return std::nullopt;
    }
}

std::optional<std::string> oly::CommandLineManager::getString(std::string_view name) const noexcept(true)
{
    const auto [found, it] = estd::find(m_args, name);

    return found && it->second.isString() ? std::optional{ it->second.asString() } : std::nullopt;
}
