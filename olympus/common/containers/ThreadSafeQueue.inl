#pragma once

BeginNamespaceOlympus

template <class T>
template <class TValue>
void ThreadSafeQueue<T>::push(TValue&& value)
{
    std::lock_guard lg(m_mutex);

    m_queue.push(std::forward<TValue>(value));

    m_conditionVar.notify_one();
}

template <class T>
void ThreadSafeQueue<T>::waitAndPop(T& value)
{
    std::unique_lock lk(m_mutex);
    m_conditionVar.wait(lk, [this] { return !m_queue.empty(); });
    value = std::move(m_queue.front());
    m_queue.pop();
}

template <class T>
bool ThreadSafeQueue<T>::tryPop(T& value)
{
    std::lock_guard lg(m_mutex);

    if (m_queue.empty())
    {
        return false;
    }

    value = std::move(m_queue.front());
    m_queue.pop();
    return true;
}

template <class T>
size_t ThreadSafeQueue<T>::size() const
{
    std::lock_guard lg(m_mutex);
    return m_queue.size();
}

EndNamespaceOlympus
