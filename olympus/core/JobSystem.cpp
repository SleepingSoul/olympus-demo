#include <pch.h>
#include "JobSystem.h"
#include <Job.h>

BeginNamespaceOlympus

namespace
{
    constexpr size_t getThreadCountForAffinity(JobAffinity affinity)
    {
        switch (affinity)
        {
        case JobAffinity::Render:
            // there should be only one render thread
            return 1;
        case JobAffinity::Generic:
            return 2;
        default:
            return 0;
        }
    }
}

JobSystem::JobSystem()
    : m_stopToken(false)
{
    for (int i = 0; i < EnumToNumber(JobAffinity::Count); ++i)
    {
        const auto affinity = static_cast<JobAffinity>(i);

        const auto numThreads = getThreadCountForAffinity(affinity);

        m_jobs.emplace_back(numThreads, m_queues[affinity], m_stopToken).start();
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
    std::next(m_jobs.begin(), EnumToNumber(affinity))->getEvent().signal();
}

void JobSystem::stop()
{
    if (m_stopped)
    {
        return;
    }

    m_stopToken.store(true);

    std::for_each(m_jobs.begin(), m_jobs.end(), [](JobExecution& execution) { execution.getEvent().signal(); });
    std::for_each(m_jobs.begin(), m_jobs.end(), [](JobExecution& execution) { execution.join(); });

    m_stopped = true;
}

JobSystem::JobExecution::JobExecution(size_t numThreads, JobQueue& jobQueue, StopToken& stopToken)
{
    for (size_t i = 0; i < numThreads; ++i)
    {
        m_threads.emplace_back(jobQueue, m_event, stopToken);
    }
}

void JobSystem::JobExecution::start()
{
    std::for_each(m_threads.begin(), m_threads.end(), [](JobThread& thread) { thread.start(); });
}

void JobSystem::JobExecution::join()
{
    for (JobThread& thread : m_threads)
    {
        if (!thread.isJoinable())
        {
            olyError("[JobSystem] A thread with id: {} in job system is not joinable - cannot terminate work correctly.", thread.getID());
            continue;
        }

        thread.join();
    }
}

EndNamespaceOlympus
