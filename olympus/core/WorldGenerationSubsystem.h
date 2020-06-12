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
    std::vector<Cube> generateWorld(std::uint32_t seed, const glm::vec3& eyePosition);
};

EndNamespaceOlympus
