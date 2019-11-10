#pragma once

#include <string_view>
#include <fstream>
#include <array>
#include <Windows.h>

namespace oly
{
    template <size_t TBufferSize = 256>
    class Logger
    {
    public:
        Logger(const std::string_view& id);
        ~Logger();

        template <class ...TArgs>
        void info(const std::string_view& format, TArgs&& ... args);

        template <class ...TArgs>
        void debug(const std::string_view& format, TArgs&& ... args);

        template <class ...TArgs>
        void warning(const std::string_view& format, TArgs&& ... args);

        template <class ...TArgs>
        void error(const std::string_view& format, TArgs&& ... args);

    private:
        template <class ...TArgs>
        void logImpl(const std::string_view& levelName, const std::string_view& format, TArgs&& ... args);

        std::ofstream m_outFile;
        std::array<char, TBufferSize> m_buffer;
        std::array<char, 128> m_headerBuffer;
        SYSTEMTIME m_timeBuffer;
    };
}

#include "Logger.inl"