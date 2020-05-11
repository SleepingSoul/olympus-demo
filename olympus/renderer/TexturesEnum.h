#pragma once

#include <limits>
#include <utility>

#include <utils/macros.h>

BeginNamespaceOlympus

enum class TextureType
{
    NoSignal = 0,
    Count,
    Invalid = std::numeric_limits<std::underlying_type_t<TextureType>>::max()
};

EndNamespaceOlympus
