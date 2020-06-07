#include <pch.h>
#include "RecognizeBaseJob.h"

#include <opencv2/opencv.hpp>

#include <extra_std/extra_std.h>

#include <marker_detector.h>

BeginNamespaceOlympus

RecognizeBaseJob::RecognizeBaseJob(cv::Mat&& frame, markers::MarkerRecognitionOptions& options, ThreadSafeOutputFunctor outputFunctor)
    : Base("RecognizeBase", JobAffinity::Generic)
    , m_frame(std::move(frame))
    , m_outputFunctor(std::move(outputFunctor))
    , m_options(options)
{}

void RecognizeBaseJob::execute()
{
    m_outputFunctor(markers::detectMarkers(m_options, m_frame));
}

EndNamespaceOlympus
