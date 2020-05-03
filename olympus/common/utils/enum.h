#pragma once

#include <type_traits>

#include <utils/macros.h>

BeginNamespaceOlympus

template <class TEnum>
[[nodiscard]] constexpr auto EnumToNumber(TEnum enumValue)
{
    static_assert(std::is_enum_v<TEnum>);

    return std::underlying_type_t<TEnum>(enumValue);
}

EndNamespaceOlympus
