#pragma once

#include <EngineSubsystem.h>

BeginNamespaceOlympus

class WorldGenerationSubsystem : public EngineSubsystem
{
public:
    using Base = EngineSubsystem;

    WorldGenerationSubsystem(EngineImpl& engine);

    void update() override;
};

EndNamespaceOlympus
