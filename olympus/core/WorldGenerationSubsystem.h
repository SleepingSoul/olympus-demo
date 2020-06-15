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
    void generateWorldToBuffer(std::uint32_t seed, const glm::vec3& eyePosition);
    void makeWaterIfNeeded(Cube& cube, size_t index);

    std::vector<Cube> m_worldBuffer;
};

EndNamespaceOlympus
