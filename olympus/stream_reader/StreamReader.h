#pragma once

#include <atomic>
#include <mutex>
#include <vector>
#include <functional>

#include <utils/macros.h>

#include <stream_decoder.h>

BeginNamespaceOlympus

class StreamReader
{
    OlyNonCopyableMovable(StreamReader)
public:
    using Buffer = stream_decoder::Buffer;
    using StopToken = std::atomic_bool;
    using OnFrameReadyCallback = std::function<void(Buffer&&)>;

    struct ClientData
    {
        Buffer buffer;
        OnFrameReadyCallback onFrameReady{ nullptr };
    };

    StreamReader();
    ~StreamReader();

    // Blocking operation. Will start continiously reading the videostream.
    void readStream();

    void setOnFrameReady(OnFrameReadyCallback callback);

    // Will set the stop flag, so 'readStream' will stop sometime.
    void stop();

private:
    StopToken m_stopToken{ false };
    
    mutable ClientData m_clientData;
};

EndNamespaceOlympus
