#include <pch.h>
#include "AsyncVideostreamListener.h"

#include <StreamDecoder.h>

#include <Engine.h>
#include <JobSystem.h>
#include <DecodeFrameJob.h>

BeginNamespaceOlympus

AsyncVideostreamListener::AsyncVideostreamListener()
{
    m_streamReader.setOnFrameReady([this](Buffer&& frame) { onFrameReady(std::move(frame)); });
}

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
    std::lock_guard lg(m_mutex);

    return m_latestFrame;
}

void AsyncVideostreamListener::onFrameReady(Buffer&& frame)
{
    auto decodeFrameJob = std::make_unique<DecodeFrameJob>(std::move(frame), [this](cv::Mat&& result)
    {
        std::lock_guard lg(m_mutex);
        m_latestFrame = std::move(result);
    });

    olyEngine.getJobSystem().addJob(std::move(decodeFrameJob));
}

EndNamespaceOlympus
