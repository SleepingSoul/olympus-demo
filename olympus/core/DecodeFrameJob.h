#pragma once

#include <opencv2/opencv.hpp>

#include <stream_decoder.h>

#include <Job.h>

BeginNamespaceOlympus

class DecodeFrameJob : public Job
{
public:
    using Base = Job;
    using ThreadSafeOutputFunctor = std::function<void(cv::Mat&&)>;

    DecodeFrameJob(stream_decoder::Buffer&& frame, cv::Mat&& cameraMatrix, cv::Mat&& distortionMatrix, ThreadSafeOutputFunctor outputFunctor);

    void execute() override;

private:
    stream_decoder::Buffer m_frame;

    cv::Mat m_cameraMatrix;
    cv::Mat m_distortionMatrix;

    ThreadSafeOutputFunctor m_outputFunctor;
};

EndNamespaceOlympus
