#include <pch.h>
#include "AsyncVideostreamListener.h"

#include <StreamDecoder.h>

BeginNamespaceOlympus

AsyncVideostreamListener::~AsyncVideostreamListener()
{
    if (m_started.load())
    {
        stop();
    }

    if (m_streamThread.joinable())
    {
        m_streamThread.join();
    }
}

void AsyncVideostreamListener::start()
{
    if (m_started.load())
    {
        olyError("[AsyncVideostreamListener] Attempt to start videostream when it is still in progress.");
        return;
    }

    m_streamThread = std::thread([this]
    {
        logging::info("[AsyncVideostreamListener] Starting listening.");

        m_started.store(true);
        m_streamReader.readStream();
        m_started.store(false);

        logging::info("[AsyncVideostreamListener] Finished listening.");
    });
}

void AsyncVideostreamListener::stop()
{
    EASY_FUNCTION();

    if (!m_started.load())
    { 
        olyError("[AsyncVideostreamListener] Attempt to stop videostream when it is already stopped.");
        return;
    }

    m_streamReader.stop();
    m_streamThread.join();
}

cv::Mat AsyncVideostreamListener::getLatestFrame()
{
    if (!m_streamReader.frameReady())
    {
        return cv::Mat{};
    }

    m_streamReader.swapBuffers(m_latestDataBuffer);

    return stream_decoder::decode(m_latestDataBuffer);
}

EndNamespaceOlympus
