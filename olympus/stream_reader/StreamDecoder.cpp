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

    bool tryExtractFrame(Buffer& bytes, Buffer& buffer)
    {
        EASY_FUNCTION("bytes=%zu", bytes.size(), profiler::colors::Green);

        const auto jpegBegin = std::search(bytes.cbegin(), bytes.cend(), beginSearcher);
        const auto jpegEnd = std::search(bytes.cbegin(), bytes.cend(), endSearcher);

        const auto end = bytes.cend();

        if (jpegBegin != end && jpegEnd != end)
        {
            const auto endOfJPEG = std::next(jpegEnd, JPEGEnd.size());

            buffer.assign(jpegBegin, endOfJPEG);

            bytes.erase(bytes.cbegin(), endOfJPEG);

            return true;
        }

        return false;
    }

    cv::Mat decode(const Buffer& jpegData)
    {
        EASY_FUNCTION("size=%zu", jpegData.size(), profiler::colors::Green);

        try
        {
            return cv::imdecode(cv::Mat{ jpegData }, cv::IMREAD_COLOR);
        }
        catch (std::exception& e)
        {
            logging::error("[StreamDecoder] Error while decoding the frame: {}", e.what());
            return cv::Mat{};
        }
    }

}

EndNamespaceOlympus
