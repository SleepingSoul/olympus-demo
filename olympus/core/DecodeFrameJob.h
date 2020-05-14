#pragma once

#include <opencv2/opencv.hpp>

#include <StreamDecoder.h>

#include <Job.h>

BeginNamespaceOlympus

class DecodeFrameJob : public Job
{
public:
    using Base = Job;
    using ThreadSafeOutputFunctor = std::function<void(cv::Mat&&)>;

    DecodeFrameJob(stream_decoder::Buffer&& frame, ThreadSafeOutputFunctor outputFunctor);

    void execute() override;

private:
    stream_decoder::Buffer m_frame;

    ThreadSafeOutputFunctor m_outputFunctor;
};

EndNamespaceOlympus
