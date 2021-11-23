#include <pch.h>
#include "WorldGenerationSubsystem.h"

#include <easy/profiler.h>

#include <PerlinNoise/PerlinNoise.hpp>

#include <utils/math.h>

#include <EngineImpl.h>

BeginNamespaceOlympus

namespace
{
    const std::array Colors{
        glm::vec4{ 1.f, 0.f, 0.f, 1.f },
        glm::vec4{ 0.f, 1.f, 0.f, 1.f },
        glm::vec4{ 0.f, 0.f, 1.f, 1.f },
        glm::vec4{ 1.f, 1.f, 0.f, 1.f },
        glm::vec4{ 0.f, 1.f, 1.f, 1.f },
        glm::vec4{ 1.f, 0.f, 1.f, 1.f }
    };
}

WorldGenerationSubsystem::WorldGenerationSubsystem(EngineImpl& engine)
    : Base(engine)
    , m_sylv("data/models/sylvanas.obj")
{}

void WorldGenerationSubsystem::update()
{
    EASY_FUNCTION(profiler::colors::Grey600);

    Base::update();

    auto& renderer = m_engine.getRenderer();

    const auto& markers = m_engine.getMarkers();

    for (size_t i = 0; i < markers.size(); ++i)
    {
        EASY_BLOCK("Generate world for one marker", profiler::colors::Grey700);

        AnyModelRendererComponent::WorldModel model;

        markers[i].projectionMatrix.copyTo(model.projection);
        markers[i].modelviewMatrix.copyTo(model.modelView);

        model.model = &m_sylv;

        renderer.getAnyModelRenderComponent().renderModels(std::move(model));
    }
}

 
EndNamespaceOlympus
