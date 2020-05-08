#pragma once

BeginNamespaceOlympus

class JobEvent
{
    // Cannot be copied/moved because of condition variable
    OlyNonCopyableMovable(JobEvent)
public:
    JobEvent() = default;
    // called from the consumer (worker) thread
    void waitAndReset();

    // called from the producer thread
    void signal();

    void reset();

private:
    void set(bool isSet);

    bool m_isSet{ false };
    std::mutex m_mutex;
    std::condition_variable m_condition;
};

EndNamespaceOlympus
