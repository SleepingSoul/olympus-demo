#pragma once

#include <containers/ThreadSafeQueue.h>

BeginNamespaceOlympus

class Job;
class JobEvent;

class JobThread
{
    OlyNonCopyable(JobThread)
public:
    using JobQueue = ThreadSafeQueue<std::unique_ptr<Job>>;
    using StopToken = std::atomic_bool;

    JobThread(JobQueue& jobQueue, JobEvent& jobEvent, StopToken& stopToken);

    void start();
    void join() { m_thread.join(); }
    bool isJoinable() const { return m_thread.joinable(); }

private:
    void pollAndExecuteJobs();

    JobQueue& m_sharedJobQueue;
    JobEvent& m_jobEvent;
    StopToken& m_stopToken;

    std::thread m_thread;
};

EndNamespaceOlympus
