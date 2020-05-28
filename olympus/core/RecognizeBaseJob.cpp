#include <pch.h>
#include "RecognizeBaseJob.h"

#include <opencv2/opencv.hpp>

#include <extra_std/extra_std.h>

#include <glyph_detector.h>

BeginNamespaceOlympus

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
