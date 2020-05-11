#include "StreamReader.h"
#include <curl/curl.h>

#include <string>
#include <algorithm>
#include <string_view>

#include <easy/profiler.h>

#include <extra_std/extra_std.h>

#include <managers/CommandLineManager.h>
#include <logging/logging.h>
#include <utils/olyerror.h>

BeginNamespaceOlympus

namespace
{
    constexpr int CurlStopCode = -1;
    constexpr int CurlKeepTransfering = 0;

    constexpr long CurlConnectionTimeoutS = 3;

    constexpr size_t BufferCapacity = 100000;

    size_t curlWriteCallback(void* data, size_t size, size_t nmemb, void* userp)
    {
        EASY_FUNCTION("bytes=%zu", size * nmemb);

        auto& clientData = *reinterpret_cast<StreamReader::ClientData*>(userp);

        const auto* const dataBegin = reinterpret_cast<unsigned char*>(data);

        std::lock_guard lg(clientData.mutex);

        clientData.buffer.insert(clientData.buffer.end(), dataBegin, std::next(dataBegin, size * nmemb));

        if (stream_decoder::tryExtractFrame(clientData.buffer, clientData.jpegBytes))
        {
            logging::debug("[StreamReader] JPEG frame exctracted.");
        }

        return size * nmemb;
    }

    int curlProgressCallback(void* clientp, curl_off_t, curl_off_t, curl_off_t, curl_off_t)
    {
        EASY_FUNCTION();

        const auto& stopToken = *reinterpret_cast<StreamReader::StopToken*>(clientp);

        if (stopToken.load())
        {
            logging::info("[StreamReader] Stopping receiving the stream.");
            return CurlStopCode;
        }

        return CurlKeepTransfering;
    }

    bool validateCredentials(std::string_view credentials)
    {
        const auto [found, it] = estd::find(credentials, ':');

        const auto distance = static_cast<size_t>(std::distance(credentials.cbegin(), it));

        return found && distance != 0 && distance < credentials.size();
    }

    std::string hidePassword(std::string credentials)
    {
        const auto [found, it] = estd::find(credentials, ':');

        std::fill(std::next(it), credentials.end(), '*');

        return credentials;
    }
}

StreamReader::StreamReader()
{
    m_clientData.buffer.reserve(BufferCapacity);
    m_clientData.jpegBytes.reserve(BufferCapacity);

    curl_global_init(CURL_GLOBAL_WIN32);
}

StreamReader::~StreamReader()
{
    m_stopToken.store(true);

    curl_global_cleanup();
}

void StreamReader::readStream()
{
    EASY_FUNCTION(profiler::colors::LightGreen);

    m_stopToken.store(false);

    const auto streamURL = olyCommandLineManager.getString(CommandLineOptions::StreamURL);
    const auto credentials = olyCommandLineManager.getString(CommandLineOptions::StreamCredentials);

    if (!streamURL)
    {
        olyError("[StreamReader] No stream URL specified, cannot read the stream.");
        return;
    }

    CURL* const handle = curl_easy_init();

    if (!handle)
    {
        olyError("[StreamReader] Handle is nullptr");
        return;
    }

    const std::array results = {
        curl_easy_setopt(handle, CURLOPT_URL, streamURL->c_str()),
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, &m_clientData),
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, curlWriteCallback),
        curl_easy_setopt(handle, CURLOPT_XFERINFODATA, &m_stopToken),
        curl_easy_setopt(handle, CURLOPT_XFERINFOFUNCTION, curlProgressCallback),
        curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0),
        curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, CurlConnectionTimeoutS)
    };


    for (auto it = results.cbegin(); it != results.cend(); ++it)
    {
        if (*it != CURLE_OK)
        {
            olyError("[StreamReader] curl option '{}' wasn't set due to error code: {}", std::distance(results.cbegin(), it), *it);
        }
    }

    if (credentials && validateCredentials(*credentials))
    {
        logging::info("[StreamReader] Using credentials: {}", hidePassword(*credentials));

        curl_easy_setopt(handle, CURLOPT_USERPWD, credentials->c_str());
    }
    else
    {
        logging::warning("[StreamReader] No credentials specified or they are invalid. Will attempt to connect without them.");
    }

    const CURLcode result = curl_easy_perform(handle);

    curl_easy_cleanup(handle);

    if (result != CURLE_OK && result != CURLE_ABORTED_BY_CALLBACK)
    {
        olyError("[StreamReader] URL stream read operation ended with error. Code: {}", result);
    }
}

void StreamReader::swapBuffers(Buffer& outBuffer)
{
    EASY_FUNCTION(profiler::colors::DarkGreen);

    outBuffer.clear();

    if (outBuffer.capacity() != BufferCapacity)
    {
        outBuffer.reserve(BufferCapacity);
    }

    std::lock_guard lg(m_clientData.mutex);

    m_clientData.jpegBytes.swap(outBuffer);
}

bool StreamReader::frameReady() const
{
    std::lock_guard lg(m_clientData.mutex);

    return !m_clientData.jpegBytes.empty();
}

void StreamReader::stop()
{
    m_stopToken.store(true);
}

EndNamespaceOlympus
