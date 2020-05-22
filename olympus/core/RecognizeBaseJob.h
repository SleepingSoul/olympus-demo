#pragma once

#include <opencv2/opencv.hpp>

#include <StreamDecoder.h>

#include <Job.h>

BeginNamespaceOlympus

class RecognizeBaseJob : public Job
{
public:
    using Base = Job;
    using ThreadSafeOutputFunctor = std::function<void(std::vector<cv::Vec2i>&&)>;

    RecognizeBaseJob(cv::Mat&& frame, ThreadSafeOutputFunctor outputFunctor);

    void execute() override;

private:
    cv::Mat m_frame;

    ThreadSafeOutputFunctor m_outputFunctor;
};

EndNamespaceOlympus
