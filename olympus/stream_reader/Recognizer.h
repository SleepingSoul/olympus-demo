#pragma once

#include <vector>

#include <utils/macros.h>

#include <RecognitionOptions.h>

BeginNamespaceOlympus

namespace recognizer
{
    std::vector<cv::Vec2i> recognizeBase(const RecognitionOptions& options, const cv::Mat& frame);
}

EndNamespaceOlympus
