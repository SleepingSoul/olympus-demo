#pragma once

#include <OpenGLRenderer.h>

#include <EngineSubsystem.h>

BeginNamespaceOlympus

class WorldGenerationSubsystem : public EngineSubsystem
{
public:
    using Base = EngineSubsystem;

    WorldGenerationSubsystem(EngineImpl& engine);

    void update() override;

private:
    Model m_sylv;
};

EndNamespaceOlympus
