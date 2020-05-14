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

    AsyncVideostreamListener();
    ~AsyncVideostreamListener();

    void start();
    void stop();

    cv::Mat getLatestFrame();

    bool isListening() const { return m_started.load(); }

private:
    void onFrameReady(Buffer&& frame);

    StreamReader m_streamReader;
    
    cv::Mat m_latestFrame;
    std::mutex m_mutex;

    std::thread m_streamThread;
    std::atomic_bool m_started{ false };
};

EndNamespaceOlympus
