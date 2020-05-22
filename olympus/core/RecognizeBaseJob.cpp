#include <pch.h>
#include "RecognizeBaseJob.h"

#include <opencv2/opencv.hpp>

#include <Recognizer.h>
#include <RecognitionOptions.h>

BeginNamespaceOlympus

namespace
{
    recognizer::RecognitionOptions getOptions()
    {
        recognizer::RecognitionOptions options;

        options.contourApproxMode = cv::ContourApproximationModes::CHAIN_APPROX_TC89_L1;
        options.contourRetrievalMode = cv::RetrievalModes::RETR_CCOMP;
        options.numContours = 4;
        options.preprocessingBlurRadius = 5;
        options.lowerHSVBegin = { 0, 100, 100 };
        options.lowerHSVEnd = { 10, 255, 255 };
        options.upperHSVBegin = { 160, 100, 100 };
        options.upperHSVEnd = { 179, 255, 255 };
        
        return options;
    }
}

RecognizeBaseJob::RecognizeBaseJob(cv::Mat&& frame, ThreadSafeOutputFunctor outputFunctor)
    : Base("RecognizeBase", JobAffinity::Generic)
    , m_frame(std::move(frame))
    , m_outputFunctor(std::move(outputFunctor))
{}

void RecognizeBaseJob::execute()
{
    m_outputFunctor(recognizer::recognizeBase(getOptions(), m_frame));
}

EndNamespaceOlympus
