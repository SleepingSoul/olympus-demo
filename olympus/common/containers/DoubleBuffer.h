#pragma once

#include <extra_std/extra_std.h>

#include <utils/macros.h>
#include <utils/olyerror.h>

namespace estd
{
    ESTD_HAS_METHOD(swap)
    ESTD_HAS_METHOD(clear)
    ESTD_HAS_METHOD(empty)
}

BeginNamespaceOlympus

template <class TContainer>
class DoubleBuffer
{
public:
    template <class ...TArgs>
    explicit DoubleBuffer(TArgs&&... args)
        : m_front(std::forward<TArgs>(args)...)
    {
        static_assert(estd::is_container_v<TContainer>);
        static_assert(std::is_default_constructible_v<TContainer>);
        static_assert(estd::has_method_swap_v<TContainer, TContainer&> || std::is_swappable_v<TContainer>);
        static_assert(estd::has_method_clear_v<TContainer>);
    }

    void swapBuffers()
    {
        if constexpr (estd::has_method_swap_v<TContainer, TContainer&>)
        {
            m_front.swap(m_back);
        }
        else
        {
            std::swap(m_front, m_back);
        }
    }

    void swapBuffersSafe()
    {
        static_assert(estd::has_method_empty_v<TContainer>);

        if (!m_back.empty())
        {
            olyError("[DoubleBuffer] Swapping buffers when back buffer still contains elements.");
            clearBackBuffer();
        }

        swapBuffers();
    }

    TContainer& frontBuffer() { return m_front; }
    const TContainer& frontBuffer() const { return m_front; }

    TContainer& backBuffer() { return m_back; }
    const TContainer& backBuffer() const { return m_back; }

    void clearBackBuffer()
    {
        m_back.clear();
    }

private:
    TContainer m_front;
    TContainer m_back;
};

EndNamespaceOlympus
