#pragma once

#include <thread>

#include <utils/macros.h>

BeginNamespaceOlympus

namespace threading
{
    // This functions are not thread safe

    void defineCurrentThreadAsMain();
    bool isMainThread();
    std::thread::id getMainThreadID();
}

EndNamespaceOlympus
