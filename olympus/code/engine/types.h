#pragma once

#include "stdafx.h"

namespace oly
{
    // containers
    using Vector = std::vector;
    using Array = std::array;
    using String = std::string;
    using StringView = std::string_view;

    // integral types
    using int32 = std::int32_t;
    using uint32 = std::uint32_t;
    using byte = std::uint8_t;
    using int8 = std::int8_t;
    using uint8 = std::uint8_t;
    using int64 = std::int64_t;
    using uint64 = std::uint64_t;

    // floating point types
    using float32 = float;
    static_assert(sizeof(float) == 4);

    using float64 = double;
    static_assert(sizeof(double) == 8);
}
