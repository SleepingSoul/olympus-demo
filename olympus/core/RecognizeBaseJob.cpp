#include <pch.h>
#include "RecognizeBaseJob.h"

#include <opencv2/opencv.hpp>

#include <extra_std/extra_std.h>

#include <Recognizer.h>
#include <RecognitionOptions.h>
#include <glyph_detector.h>

BeginNamespaceOlympus

namespace
{
    recognizer::RecognitionOptions getOptions()
    {
        recognizer::RecognitionOptions options;

        options.contourApproxMode = cv::ContourApproximationModes::CHAIN_APPROX_TC89_L1;
        options.contourRetrievalMode = cv::RetrievalModes::RETR_CCOMP;
        options.numContours = 4;
        options.preprocessingBlurRadius = 0;
        options.lowerHSVBegin = { 0, 100, 100 };
        options.lowerHSVEnd = { 10, 255, 255 };
        options.upperHSVBegin = { 160, 100, 100 };
        options.upperHSVEnd = { 179, 255, 255 };
        
        return options;
    }

    int distance(const cv::Point2f& a, const cv::Point2f& b)
    {
        const auto diff = a - b;

        return std::sqrt(diff.x * diff.x + diff.y * diff.y);
    }

    cv::Point2f nearestFrameEdge(const cv::Point2f& point, const cv::Size& frameSize)
    {
        const std::array edges = {
            cv::Point2f(0, 0), cv::Point2f(frameSize.width, 0), cv::Point2f(frameSize.width, frameSize.height), cv::Point2f(0, frameSize.height)
        };

        double min = std::numeric_limits<double>::infinity();
        auto nearest = edges.cbegin();

        for (auto it = edges.begin(); it != edges.end(); ++it)
        {
            const auto dist = distance(*it, point);

            if (dist < min)
            {
                min = dist;
                nearest = it;
            }
        }

        return *nearest;
    }
}

RecognizeBaseJob::RecognizeBaseJob(cv::Mat&& frame, const glyph::GlyphRecognitionOptions& options, ThreadSafeOutputFunctor outputFunctor)
    : Base("RecognizeBase", JobAffinity::Generic)
    , m_frame(std::move(frame))
    , m_outputFunctor(std::move(outputFunctor))
    , m_options(options)
{}

void RecognizeBaseJob::execute()
{
    m_outputFunctor(glyph::detectGlyph(m_options, m_frame));
}

EndNamespaceOlympus
