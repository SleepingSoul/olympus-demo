#include <pch.h>
#include "JobThread.h"
#include <JobEvent.h>
#include <Job.h>

BeginNamespaceOlympus

JobThread::JobThread(JobQueue& jobQueue, JobEvent& jobEvent, StopToken& stopToken)
    : m_sharedJobQueue(jobQueue)
    , m_jobEvent(jobEvent)
    , m_stopToken(stopToken)
{}

void JobThread::start()
{
    m_thread = std::thread([this] { pollAndExecuteJobs(); });
}

void JobThread::pollAndExecuteJobs()
{
    while (true)
    {
        m_jobEvent.waitAndReset();

        if (m_stopToken.load())
        {
            return;
        }

        std::unique_ptr<Job> newJob;
        
        while (m_sharedJobQueue.tryPop(newJob))
        {
            if (m_stopToken.load())
            {
                return;
            }

            const bool silent = newJob->isSilent();

            if (!silent)
            {
                logging::debug("Start executing job '{}' in thread {}.", newJob->getJobID(), m_thread.get_id());
            }

            newJob->execute();

            if (!silent)
            {
                logging::debug("Job '{}' was executed.", newJob->getJobID());
            }
        }
    }
}

EndNamespaceOlympus
