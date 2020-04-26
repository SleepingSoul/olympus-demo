#include "Engine.h"
#include <EngineImpl.h>


oly::Engine& oly::Engine::instance()
{
    static Engine engine{};
    return engine;
}

void oly::Engine::initialize()
{
    m_impl = std::make_unique<EngineImpl>();
}

bool oly::Engine::initializedSuccessfuly() const
{
    return m_impl->initializedSuccessfuly();
}

int oly::Engine::run()
{
    return m_impl->run();
}
