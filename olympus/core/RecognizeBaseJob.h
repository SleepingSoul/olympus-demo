#pragma once

#include <MarkerOptions.h>
#include <marker_detector.h>
#include <stream_decoder.h>

#include <Job.h>

BeginNamespaceOlympus

class RecognizeBaseJob : public Job
{
public:
    using Base = Job;
    using ThreadSafeOutputFunctor = std::function<void(std::vector<markers::DetectResult>&&)>;

    RecognizeBaseJob(cv::Mat&& frame, markers::MarkerRecognitionOptions& options, ThreadSafeOutputFunctor outputFunctor);

    void execute() override;

private:
    cv::Mat m_frame;
    markers::MarkerRecognitionOptions& m_options;

    ThreadSafeOutputFunctor m_outputFunctor;
};

EndNamespaceOlympus
