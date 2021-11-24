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
    Model m_mercy;

    glm::vec3 sylvScale{ 0.2f, 0.2f, 0.2f };
    float sylvRotation = glm::radians(90.f);
    glm::vec3 sylvRotAxis{ 1.f, 0.f, 0.f };
    glm::vec3 sylvPos{ 0.f, 0.f, 0.f };

    glm::vec3 gengPos{ 0.f,0.f,0.f };
    glm::vec3 gengScale{ 0.1f, 0.1f,0.1f };
};

EndNamespaceOlympus
