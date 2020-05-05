#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

#include <utils/macros.h>

BeginNamespaceOlympus

template <class T>
class ThreadSafeQueue
{
    OlyNonCopyableMovable(ThreadSafeQueue)
public:
    using Base = std::queue<T>;
    using value_type = T;
    using reference_type = T&;

    ThreadSafeQueue() = default;

    template <class TElem>
    void push(TElem&& value);

    void waitAndPop(T& value);

    bool tryPop(T& value);

    size_t size() const;

private:
    std::queue<T> m_queue;
    std::condition_variable m_conditionVar;
    mutable std::mutex m_mutex;
};

EndNamespaceOlympus

#include "ThreadSafeQueue.inl"
