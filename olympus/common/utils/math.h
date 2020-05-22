#pragma once

#include <utility>

#include <utils/macros.h>

BeginNamespaceOlympus

template <class TFloat>
bool equalsWithAlpha(const TFloat& a, const TFloat& b, const TFloat& alpha)
{
    static_assert(std::is_floating_point_v<TFloat>);

    return std::abs(a - b) < alpha;
}

EndNamespaceOlympus
