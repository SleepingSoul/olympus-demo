#pragma once

#include <functional>

#include <Job.h>

BeginNamespaceOlympus

class DelegateJob : public Job
{
public:
    using Base = Job;
    using Executor = std::function<void()>;

    explicit DelegateJob(JobAffinity affinity, Executor executor)
        : Base(affinity)
        , m_executor(std::move(executor))
    {}

    void execute() override { m_executor(); }

private:
    Executor m_executor;
};

EndNamespaceOlympus
