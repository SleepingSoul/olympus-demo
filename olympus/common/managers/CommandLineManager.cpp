#include "CommandLineManager.h"
#include <map>
#include <extra_std/extra_std.h>
#include <logging/logging.h>
#include <utils/olyerror.h>

BeginNamespaceOlympus

namespace
{
    const bool ShowHelp = true;

    auto Usage =
        R"(olympus
Usage:
    core.exe --stream-url=url [--width=w --height=h --saveprofile=filename --stream-creds=<login:password>]
Options:
    -h, --help                              Show this screen.
    --width=<px>                            Screen width in pixels [default: 800].
    --height=<px>                           Screen height in pixels [default: 600].
    --saveprofile=<filename>                This option works only if program is build in PROFILE configuration. If specified, profiled data will be collected and saved to the file.
    --stream-url=<url>                      URL of the videostream.
    --stream-creds=<login:password>         Crenedtials of the videostream.
        )";
}

CommandLineManager& CommandLineManager::instance()
{
    static oly::CommandLineManager cmdManager{};
    return cmdManager;
}

void CommandLineManager::initialize(int argc, char** argv)
{
    auto args = docopt::docopt(Usage, { std::next(argv), std::next(argv, argc) }, ShowHelp, "debug version");

    m_args.insert(std::make_move_iterator(args.begin()), std::make_move_iterator(args.end()));
}

std::optional<long> CommandLineManager::getLong(std::string_view name) const
{
    std::lock_guard lg(m_mutex);

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

std::optional<bool> CommandLineManager::getBool(std::string_view name) const
{
    std::lock_guard lg(m_mutex);

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

std::optional<std::string> CommandLineManager::getString(std::string_view name) const
{
    std::lock_guard lg(m_mutex);

    const auto [found, it] = estd::find(m_args, name);

    return found && it->second.isString() ? std::optional{ it->second.asString() } : std::nullopt;
}

EndNamespaceOlympus
