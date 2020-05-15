#include "StreamDecoder.h"

#include <array>
#include <algorithm>
#include <functional>

#include <easy/profiler.h>
#include <easy/arbitrary_value.h>

#include <logging/logging.h>
#include <utils/olyerror.h>

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
        EASY_FUNCTION(profiler::colors::Green);

        const auto jpegBegin = std::search(bytes.cbegin(), bytes.cend(), beginSearcher);

        // TODO: look for end of jpeg from the other side, should be a bit faster
        const auto jpegEnd = std::search(bytes.cbegin(), bytes.cend(), endSearcher);

        const auto end = bytes.cend();

        const bool beginEndFound = (jpegBegin == end || jpegEnd == end);

        if (jpegBegin == end || jpegEnd == end)
        {
            return std::nullopt;
        }

        EASY_VALUE("bytes", bytes.size());

        if (const auto size = std::distance(jpegBegin, jpegEnd); size < 0 || size > 200000)
        {
            olyError("[StreamDecoder] Something wrong with the JPEG. Distance between begin and end is: {}", size);
        }

        const auto endOfJPEG = std::next(jpegEnd, JPEGEnd.size());

        Buffer result(jpegBegin, endOfJPEG);

        bytes.erase(bytes.cbegin(), endOfJPEG);

        return result;
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
