#pragma once

BeginNamespaceOlympus

class EngineImpl;

class EngineSubsystem
{
public:
    EngineSubsystem(EngineImpl& engine);

    virtual void update();

protected:
    EngineImpl& m_engine;
};

EndNamespaceOlympus
