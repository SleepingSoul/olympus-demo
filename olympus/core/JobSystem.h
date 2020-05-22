#pragma once

#include <JobAffinity.h>
#include <JobThread.h>
#include <JobEvent.h>

BeginNamespaceOlympus

class Job;

class JobSystem
{
    OlyNonCopyableMovable(JobSystem)
public:
    using JobQueue = JobThread::JobQueue;
    using StopToken = JobThread::StopToken;

    JobSystem();
    ~JobSystem();

    void addJob(std::unique_ptr<Job>&& job);
    void stop();

private:
    class JobExecution
    {
        OlyNonCopyable(JobExecution)
    public:
        JobExecution(size_t numThreads, JobQueue& jobQueue, StopToken& stopToken);

        void start();
        void join();

        auto& getEvent() { return m_event; }

    private:
        JobEvent m_event;
        std::list<JobThread> m_threads;
    };

    std::map<JobAffinity, JobQueue> m_queues;
    std::list<JobExecution> m_jobs;
    StopToken m_stopToken;
    bool m_stopped{ false };
};

EndNamespaceOlympus
