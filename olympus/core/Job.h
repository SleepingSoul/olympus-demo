#pragma once

#include <utils/macros.h>
#include <logging/logging.h>

#include <JobAffinity.h>

BeginNamespaceOlympus

class Job
{
public:
    explicit Job(const char* jobID, JobAffinity affinity) noexcept(true)
        : m_affinity(affinity)
        , m_jobID(jobID)
    {}

    virtual ~Job() = default;

    auto getAffinity() const { return m_affinity; }
    auto getJobID() const { return m_jobID; }

    virtual void execute() = 0;

protected:
    struct JobExecutionGuard
    {
    public:
        JobExecutionGuard(Job& job) : m_job(job) { logging::debug("Start executing job '{}' in thread {}", m_job.getJobID(), std::this_thread::get_id()); }
        ~JobExecutionGuard() { logging::debug("End executing job '{}'", m_job.getJobID()); }
    private:
        Job& m_job;
    };

private:
    JobAffinity m_affinity;
    const char* m_jobID;
};

EndNamespaceOlympus
