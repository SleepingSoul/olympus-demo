#pragma once

#include <utils/macros.h>

#include <JobAffinity.h>

BeginNamespaceOlympus

class Job
{
public:
    explicit Job(JobAffinity affinity) noexcept(true)
        : m_affinity(affinity)
    {}

    virtual ~Job() = default;

    auto getAffinity() const { return m_affinity; }
    virtual void execute() = 0;

private:
    JobAffinity m_affinity;
};

EndNamespaceOlympus
