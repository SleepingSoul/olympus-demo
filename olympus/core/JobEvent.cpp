#include <pch.h>
#include "JobEvent.h"

BeginNamespaceOlympus

void JobEvent::waitAndReset()
{
    std::unique_lock lk(m_mutex);
    m_condition.wait(lk, [this] { return m_isSet.load(); });
    m_isSet.store(false);
}

void JobEvent::signal()
{
    m_isSet.store(true);
    m_condition.notify_all();
}

void JobEvent::reset()
{
    m_isSet.store(false);
}

EndNamespaceOlympus
