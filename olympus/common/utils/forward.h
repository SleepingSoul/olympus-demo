#pragma once

#include <utility>

#include <extra_std/extra_std.h>

#include <utils/macros.h>

#define OlyPerfectForwardInContainer(methodName, containerName) \
template <class TValue> \
void methodName(TValue&& value) \
{ \
    if constexpr (estd::is_container_v<TValue>) \
    { \
        if constexpr (std::is_rvalue_reference_v<TValue>) \
        { \
            containerName.insert(containerName.cend(), std::make_move_iterator(value.begin()), std::make_move_iterator(value.end())); \
        } \
        else \
        { \
            containerName.insert(containerName.cend(), value.cbegin(), value.cend()); \
        } \
    } \
    else \
    { \
        containerName.emplace_back(std::forward<TValue>(value)); \
    } \
} \

