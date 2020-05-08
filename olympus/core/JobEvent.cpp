#include <pch.h>
#include "JobEvent.h"

BeginNamespaceOlympus

void JobEvent::waitAndReset()
{
    std::unique_lock lk(m_mutex);

    m_condition.wait(lk, [this] { return m_isSet; });

    m_isSet = false;
}

void JobEvent::signal()
{
    set(true);
    m_condition.notify_one();
}

void JobEvent::reset()
{
    set(false);
}

void JobEvent::set(bool isSet)
{
    std::lock_guard lg(m_mutex);
    m_isSet = isSet;
}

EndNamespaceOlympus
