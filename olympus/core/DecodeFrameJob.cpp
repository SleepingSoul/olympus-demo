#include <pch.h>
#include "DecodeFrameJob.h"

#include <easy/profiler.h>

BeginNamespaceOlympus

DecodeFrameJob::DecodeFrameJob(stream_decoder::Buffer&& frame, cv::Mat&& cameraMatrix, cv::Mat&& distortionMatrix, ThreadSafeOutputFunctor outputFunctor)
    : Base("Decode frame job", JobAffinity::Generic, true)
    , m_frame(std::move(frame))
    , m_cameraMatrix(std::move(cameraMatrix))
    , m_distortionMatrix(std::move(distortionMatrix))
    , m_outputFunctor(std::move(outputFunctor))
{}

void DecodeFrameJob::execute()
{
    EASY_FUNCTION(profiler::colors::Green900);

    cv::Mat result, distorted;

    try
    {
        result = stream_decoder::decode(m_frame);

        if (!m_cameraMatrix.empty() && !m_distortionMatrix.empty())
        {
            EASY_BLOCK("Undistort", profiler::colors::LightGreen900);
            cv::undistort(result, distorted, m_cameraMatrix, m_distortionMatrix);
        }
        else
        {
            distorted = result;
        }
    }
    catch (std::exception& e)
    {
        olyError("[DecodeFrameJob] Decoding error: {}", e.what());
    }

    m_outputFunctor(std::move(distorted));
}

EndNamespaceOlympus

