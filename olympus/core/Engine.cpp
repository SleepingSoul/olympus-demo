#include <pch.h>
#include "Engine.h"
#include <EngineImpl.h>

BeginNamespaceOlympus

Engine& Engine::instance()
{
    static Engine engine{};
    return engine;
}

void Engine::initialize()
{
    m_impl = std::make_unique<EngineImpl>();
    m_impl->initialize();
}

bool Engine::initializedSuccessfuly() const
{
    return m_impl->initializedSuccessfuly();
}

AsyncVideostreamListener& Engine::getAsyncVideostreamListener()
{
    return m_impl->getAsyncVideostreamListener();
}

JobSystem& Engine::getJobSystem()
{
    return m_impl->getJobSystem();
}

int Engine::run()
{
    return m_impl->run();
}

EndNamespaceOlympus
