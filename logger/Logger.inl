#include "Logger.h"


template <size_t TBufferSize>
oly::Logger<TBufferSize>::Logger(const std::string_view& id)
    : m_outFile(id.data(), std::ofstream::out)
    , m_buffer{}
{

}

template <size_t TBufferSize>
oly::Logger<TBufferSize>::~Logger()
{
    m_outFile.close();
}

template <size_t TBufferSize>
template <class ...TArgs>
void oly::Logger<TBufferSize>::info(const std::string_view& format, TArgs&& ... args)
{
#ifndef INFO_LOG_UNSUPPORTED
    logImpl("INFO", format, std::forward<TArgs>(args)...);
#endif
}

template <size_t TBufferSize>
template <class ...TArgs>
void oly::Logger<TBufferSize>::debug(const std::string_view& format, TArgs&& ... args)
{
#ifndef DEBUG_LOG_UNSUPPORTED
    logImpl("DEBUG", format, std::forward<TArgs>(args)...);
#endif
}

template <size_t TBufferSize>
template <class ...TArgs>
void oly::Logger<TBufferSize>::warning(const std::string_view& format, TArgs&& ... args)
{
#ifndef WARNING_LOG_UNSUPPORTED
    logImpl("WARNING", format, std::forward<TArgs>(args)...);
#endif
}

template <size_t TBufferSize>
template <class ...TArgs>
void oly::Logger<TBufferSize>::error(const std::string_view& format, TArgs&& ... args)
{
#ifndef ERROR_LOG_UNSUPPORTED
    logImpl("ERROR", format, std::forward<TArgs>(args)...);
#endif
}

template <size_t TBufferSize>
template <class ...TArgs>
void oly::Logger<TBufferSize>::logImpl(const std::string_view& levelName, const std::string_view& format, TArgs&& ... args)
{
    GetLocalTime(&m_timeBuffer);

    const auto headerFormat = "[%d-%d-%d - %d:%d:%d][%s] ";

    const auto headerLength = sprintf_s(m_headerBuffer.data(), m_headerBuffer.size(), headerFormat
        , m_timeBuffer.wDay
        , m_timeBuffer.wMonth
        , m_timeBuffer.wYear
        , m_timeBuffer.wHour
        , m_timeBuffer.wMinute
        , m_timeBuffer.wMilliseconds
        , levelName.data());

    std::copy(m_headerBuffer.cbegin(), m_headerBuffer.cend(), m_buffer.begin());

    const auto bufferEnd = std::next(m_buffer.begin(), headerLength);

    const auto stringLength = sprintf_s(&*bufferEnd, m_buffer.size() - headerLength, format.data(), std::forward<TArgs>(args)...);

    m_buffer[stringLength + headerLength] = '\n';

    OutputDebugStringA(m_buffer.data());
    m_outFile.write(m_buffer.data(), stringLength + headerLength + 1);
}
