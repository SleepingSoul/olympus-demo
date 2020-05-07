#include "threading_utils.h"

#include <fmt/ostream.h>

#include <utils/olyerror.h>
#include <logging/logging.h>

BeginNamespaceOlympus

namespace threading
{
    namespace
    {
        std::thread::id MainThreadID{};
        const std::thread::id NoThreadID{};
    }

    void defineCurrentThreadAsMain()
    {
        if (MainThreadID != NoThreadID)
        {
            olyError("Attempt to redefine main thread detected.");
            return;
        }

        MainThreadID = std::this_thread::get_id();

        logging::info("Main thread is defined: {}", MainThreadID);
    }

    bool isMainThread()
    {
        return MainThreadID == std::this_thread::get_id();
    }

    std::thread::id getMainThreadID()
    {
        return MainThreadID;
    }
}

EndNamespaceOlympus


