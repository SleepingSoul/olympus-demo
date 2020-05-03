#pragma once

#include <functional>

#include <logging/logging.h>

#include <Job.h>

BeginNamespaceOlympus

class DelegateJob : public Job
{
public:
    using Base = Job;
    using Executor = std::function<void()>;

    explicit DelegateJob(const char* jobID, JobAffinity affinity, Executor executor)
        : Base(jobID, affinity)
        , m_executor(std::move(executor))
    {}

    void execute() override
    {
        JobExecutionGuard jeg(*this);
        m_executor();
    }

private:
    Executor m_executor;
};

EndNamespaceOlympus
