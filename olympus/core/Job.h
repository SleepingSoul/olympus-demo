#pragma once

#include <utils/macros.h>
#include <logging/logging.h>

#include <JobAffinity.h>

BeginNamespaceOlympus

class Job
{
public:
    // Remember: contructor is called in thread where job is being initialized (usually main thread)
    // destructor will be called in the thread where the job is being executed.

    explicit Job(const char* jobID, JobAffinity affinity) noexcept(true)
        : m_affinity(affinity)
        , m_jobID(jobID)
    {}

    virtual ~Job() = default;

    auto getAffinity() const { return m_affinity; }
    auto getJobID() const { return m_jobID; }

    virtual void execute() = 0;

protected:
    // It is recommended to use this RAII class in "execute" to ensure job execution will be logged.
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