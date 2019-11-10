#pragma once

namespace oly
{
    template <class TValue>
    using Vector = std::vector<TValue>;

    template <class TValue, size_t Size>
    using Array = std::array<TValue, Size>;

    using String = std::string;
    using StringView = std::string_view;

    using Logger = spdlog::Logger;
}
