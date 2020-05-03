#pragma once

#include <limits>
#include <utils/macros.h>

BeginNamespaceOlympus

enum class JobAffinity : unsigned char
{
    Render, // job will be executed on the render-specific thread (it is required by render API, render operations that use OpenGL API can only be executed in the thread)
            // where OpenGL was initialized.
    Generic,
    Count,
    Invalid = std::numeric_limits<unsigned char>::max()
};

EndNamespaceOlympus
