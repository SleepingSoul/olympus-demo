#include "stream_decoder.h"

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
        const std::boyer_moore_searcher endSearcher(JPEGEnd.rbegin(), JPEGEnd.rend());
    }

    std::optional<Buffer> tryExtractFrame(Buffer& bytes)
    {
        EASY_FUNCTION(profiler::colors::Green);

        const auto jpegBegin = std::search(bytes.cbegin(), bytes.cend(), beginSearcher);

        const auto jpegEnd = std::search(bytes.crbegin(), bytes.crend(), endSearcher);

        if (jpegBegin == bytes.cend() || jpegEnd == bytes.crend())
        {
            return std::nullopt;
        }

        EASY_VALUE("bytes", bytes.size());

        if (const auto size = std::distance(jpegEnd, std::make_reverse_iterator(jpegBegin)); size < 0)
        {
            logging::error("[StreamDecoder] Broken stream frame detected. Begin-end distance is: {}", size);

            bytes.erase(bytes.cbegin(), jpegEnd.base());

            return std::nullopt;
        }

        Buffer result(jpegBegin, jpegEnd.base());

        bytes.erase(bytes.cbegin(), jpegEnd.base());

        return result;
    }

    cv::Mat decode(const Buffer& jpegData)
    {
        EASY_FUNCTION("size=%zu", jpegData.size(), profiler::colors::Green);

        return cv::imdecode(cv::Mat{ jpegData }, cv::IMREAD_COLOR);
    }

}

EndNamespaceOlympus
