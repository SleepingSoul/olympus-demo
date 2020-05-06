#include <pch.h>
#include "JobSystem.h"
#include <Job.h>

BeginNamespaceOlympus

namespace
{
    constexpr size_t WorkerThreadCount = EnumToNumber(JobAffinity::Count);
}

JobSystem::JobSystem()
    : m_stopToken(false)
{
    for (size_t i = 0; i < WorkerThreadCount; ++i)
    {
        m_jobs.emplace_back(m_queues[static_cast<JobAffinity>(i)], m_stopToken).thread.start();
    }
}

JobSystem::~JobSystem()
{
    stop();
}

void JobSystem::addJob(std::unique_ptr<Job>&& job)
{
    const auto affinity = job->getAffinity();

    if (affinity == JobAffinity::Invalid)
    {
        olyError("[JobSystem] A job with invalid affinity given - no job will be executed.");
        return;
    }

    m_queues[affinity].push(std::move(job));
    std::next(m_jobs.begin(), EnumToNumber(affinity))->event.signal();
}

void JobSystem::stop()
{
    if (m_stopped)
    {
        return;
    }

    m_stopToken.store(true);

    std::for_each(m_jobs.begin(), m_jobs.end(), [](auto& execution) { execution.event.signal(); });

    for (auto& execution : m_jobs)
    {
        if (!execution.thread.isJoinable())
        {
            olyError("[JobSystem] A thread in job system is not joinable - cannot terminate work correctly.");
            continue;
        }

        execution.thread.join();
    }

    m_stopped = true;
}

EndNamespaceOlympus
