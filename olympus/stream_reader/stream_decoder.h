#pragma once

#include <vector>
#include <optional>

#include <opencv2/opencv.hpp>

#include <utils/macros.h>

BeginNamespaceOlympus

namespace stream_decoder
{
    using Buffer = std::vector<char>;

    std::optional<Buffer> tryExtractFrame(Buffer& bytes);

    cv::Mat decode(const Buffer& data);
}

EndNamespaceOlympus
