#pragma once

#include <JobAffinity.h>
#include <JobThread.h>
#include <JobEvent.h>

BeginNamespaceOlympus

class Job;

class JobSystem
{
public:
    using JobQueue = JobThread::JobQueue;
    using StopToken = JobThread::StopToken;

    JobSystem();
    ~JobSystem();

    void addJob(std::unique_ptr<Job>&& job);

private:
    struct JobExecution
    {
        JobExecution(JobQueue& jobQueue, StopToken& stopToken)
            : thread(jobQueue, event, stopToken)
        {}

        JobEvent event;
        JobThread thread;
    };

    std::map<JobAffinity, JobQueue> m_queues;
    std::vector<std::unique_ptr<JobExecution>> m_jobs;
    StopToken m_stopToken;
};

EndNamespaceOlympus
