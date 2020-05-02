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
}

bool Engine::initializedSuccessfuly() const
{
    return m_impl->initializedSuccessfuly();
}

int Engine::run()
{
    return m_impl->run();
}

EndNamespaceOlympus
