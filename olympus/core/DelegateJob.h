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

    explicit DelegateJob(const char* jobID, JobAffinity affinity, Executor executor, bool silent = false)
        : Base(jobID, affinity, silent)
        , m_executor(std::move(executor))
    {}

    void execute() override
    {
        m_executor();
    }

private:
    Executor m_executor;
};

class DelegateJobWithPromise : public DelegateJob
{
public:
    using Base = DelegateJob;

    using Base::Base;

    std::future<void> getJobEndFuture() { return m_jobEndPromise.get_future(); }

    void execute() override
    {
        try
        {
            Base::execute();
            m_jobEndPromise.set_value();
        }
        catch (...)
        {
            m_jobEndPromise.set_exception(std::current_exception());
        }
    }

private:
    std::promise<void> m_jobEndPromise;
};

EndNamespaceOlympus
