#pragma once

#include <opencv2/opencv.hpp>

#include <StreamReader.h>

BeginNamespaceOlympus

class AsyncVideostreamListener
{
    OlyNonCopyableMovable(AsyncVideostreamListener)
public:
    using StopToken = StreamReader::StopToken;
    using Buffer = StreamReader::Buffer;

    AsyncVideostreamListener() = default;
    ~AsyncVideostreamListener();

    void start();
    void stop();

    cv::Mat getLatestFrame();

    bool isListening() const { return m_started.load(); }

private:
    StreamReader m_streamReader;
    Buffer m_latestDataBuffer;

    std::thread m_streamThread;
    std::atomic_bool m_started{ false };
};

EndNamespaceOlympus
