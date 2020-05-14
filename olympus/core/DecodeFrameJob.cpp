#include <pch.h>
#include "DecodeFrameJob.h"

BeginNamespaceOlympus

DecodeFrameJob::DecodeFrameJob(stream_decoder::Buffer&& frame, ThreadSafeOutputFunctor outputFunctor)
    : Base("Decode frame job", JobAffinity::Generic)
    , m_frame(std::move(frame))
    , m_outputFunctor(std::move(outputFunctor))
{}

void DecodeFrameJob::execute()
{
    cv::Mat result;

    try
    {
        result = stream_decoder::decode(m_frame);
    }
    catch (std::exception& e)
    {
        olyError("[DecodeFrameJob] Decoding error: {}", e.what());
    }

    m_outputFunctor(std::move(result));
}

EndNamespaceOlympus

