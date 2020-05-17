#pragma once

#include <utils/macros.h>
#include <logging/logging.h>

#include <JobAffinity.h>

BeginNamespaceOlympus

class Job
{
public:
    explicit Job(const char* jobID, JobAffinity affinity, bool silent = false) noexcept(true)
        : m_affinity(affinity)
        , m_jobID(jobID)
        , m_silent(silent)
    {}

    virtual ~Job() = default;

    auto getAffinity() const { return m_affinity; }
    auto getJobID() const { return m_jobID; }
    auto isSilent() const { return m_silent; }

    void setSilent(bool silent) { m_silent = silent; }

    virtual void execute() = 0;

private:
    JobAffinity m_affinity;
    const char* m_jobID;
    bool m_silent;
};

EndNamespaceOlympus
