#pragma once

#include <optional>
#include <string_view>
#include <docopt/docopt.h>
#include <utils/macros.h>

namespace oly
{
    class CommandLineManager
    {
        OlyNonCopyableMovable(CommandLineManager)
    public:
        static CommandLineManager& instance();

        void initialize(int argc, char** argv);

        std::optional<long> getLong(std::string_view name) const noexcept(true);
        std::optional<bool> getBool(std::string_view name) const noexcept(true);
        std::optional<std::string> getString(std::string_view name) const noexcept(true);
        std::optional<std::vector<std::string>> getStringList(std::string_view name) const noexcept(true);

    private:
        CommandLineManager() = default;

        std::map<std::string, docopt::value, std::less<>> m_args;
    };
}

#define olyCommandLineManager oly::CommandLineManager::instance()
