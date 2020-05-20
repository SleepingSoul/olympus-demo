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
    using FrameID = size_t;

    AsyncVideostreamListener();
    ~AsyncVideostreamListener();

    void start();
    void stop();

    cv::Mat getLatestFrame() const;
    FrameID getLatestFrameID() const;

    bool isListening() const { return m_started.load(); }

    unsigned getStreamFPS() const;

private:
    void updateStreamFPS();

    void onFrameReady(Buffer&& frame);

    StreamReader m_streamReader;
    
    cv::Mat m_latestFrame;
    FrameID m_latestFrameID{ 0 };

    mutable std::mutex m_mutex;

    std::thread m_streamThread;
    std::atomic_bool m_started{ false };

    boost::circular_buffer<unsigned> m_streamFPS;
    std::atomic_uint m_normalizedStreamFPS{ 0u };
    double m_lastTimeStamp{ 0. };
};

EndNamespaceOlympus
