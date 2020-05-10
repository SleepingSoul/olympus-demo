#pragma once

#include <vector>

#include <opencv2/opencv.hpp>

#include <utils/macros.h>

BeginNamespaceOlympus

namespace stream_decoder
{
    using Buffer = std::vector<char>;

    bool tryExtractFrame(Buffer& bytes, Buffer& buffer);

    cv::Mat decode(const Buffer& data);
}

EndNamespaceOlympus
