#pragma once

#include <atomic>
#include <mutex>
#include <vector>

#include <utils/macros.h>

#include <StreamDecoder.h>

BeginNamespaceOlympus

class StreamReader
{
    OlyNonCopyableMovable(StreamReader)
public:
    using Buffer = stream_decoder::Buffer;
    using StopToken = std::atomic_bool;

    struct ClientData
    {
        std::mutex mutex;
        Buffer buffer;
        Buffer jpegBytes;
    };

    StreamReader();
    ~StreamReader();

    // Blocking operation. Will start continiously reading the videostream.
    void readStream();

    void swapBuffers(Buffer& outBuffer);

    // Returns true if the storage contains the data for latest frame read from the stream, or false if otherwise.
    bool frameReady() const;

    // Will set the stop flag, so 'readStream' will stop sometime.
    void stop();

private:
    StopToken m_stopToken{ false };
    
    mutable ClientData m_clientData;
};

EndNamespaceOlympus
