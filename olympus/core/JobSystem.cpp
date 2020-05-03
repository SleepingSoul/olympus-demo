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
        auto jobExecution = std::make_unique<JobExecution>(m_queues[static_cast<JobAffinity>(i)], m_stopToken);

        m_jobs.emplace_back(std::move(jobExecution))->thread.start();
    }
}

JobSystem::~JobSystem()
{
    m_stopToken.store(true);

    std::for_each(m_jobs.cbegin(), m_jobs.cend(), [](const auto& execution) { execution->event.signal(); });

    for (const auto& execution : m_jobs)
    {
        if (!execution->thread.isJoinable())
        {
            olyError("[JobSystem] A thread in job system is not joinable - cannot terminate work correctly.");
            continue;
        }

        execution->thread.join();
    }
}

void JobSystem::addJob(std::unique_ptr<Job>&& job)
{
    const auto affinity = job->getAffinity();

    if (affinity == JobAffinity::Invalid)
    {
        olyError("[JobSystem] A job with invalid affinity gived - no jon will be executed.");
        return;
    }

    m_queues[affinity].push(std::move(job));
    m_jobs[EnumToNumber(affinity)]->event.signal();
}

EndNamespaceOlympus
