#include "StreamDecoder.h"

#include <array>
#include <algorithm>
#include <functional>

#include <easy/profiler.h>

#include <logging/logging.h>

BeginNamespaceOlympus

namespace stream_decoder
{
    namespace
    {
        constexpr std::array JPEGStart = { '\xff', '\xd8' };
        constexpr std::array JPEGEnd = { '\xff', '\xd9' };

        const std::boyer_moore_searcher beginSearcher(JPEGStart.begin(), JPEGStart.end());
        const std::boyer_moore_searcher endSearcher(JPEGEnd.begin(), JPEGEnd.end());
    }

    std::optional<Buffer> tryExtractFrame(Buffer& bytes)
    {
        EASY_FUNCTION("bytes=%zu", bytes.size(), profiler::colors::Green);

        const auto jpegBegin = std::search(bytes.cbegin(), bytes.cend(), beginSearcher);
        const auto jpegEnd = std::search(bytes.cbegin(), bytes.cend(), endSearcher);

        const auto end = bytes.cend();

        if (jpegBegin != end && jpegEnd != end)
        {
            const auto endOfJPEG = std::next(jpegEnd, JPEGEnd.size());

            bytes.erase(bytes.cbegin(), endOfJPEG);

            return Buffer(jpegBegin, endOfJPEG);
        }

        return std::nullopt;
    }

    cv::Mat decode(const Buffer& jpegData)
    {
        EASY_FUNCTION("size=%zu", jpegData.size(), profiler::colors::Green);

        auto result = cv::imdecode(cv::Mat{ jpegData }, cv::IMREAD_COLOR);

        const int vertFlip = 0;

        cv::flip(result, result, vertFlip);

        return result;
    }

}

EndNamespaceOlympus
