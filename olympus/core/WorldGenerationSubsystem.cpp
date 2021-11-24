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
    , m_mercy("data/models/genji/source/Genji_1.fbx")
{
    engine.getWindowContext().addKeyboardPressCallback(GLFW_KEY_LEFT, [this]
        {
            const auto markerID = m_engine.getWindowContext().getSelectedMarker();

            if (markerID == 120)
            {
                if (m_engine.getWindowContext().getSelectedOperation() == "Move")
                {
                    sylvPos.x += 0.05f;
                }
                else if (m_engine.getWindowContext().getSelectedOperation() == "Scale")
                {
                    sylvScale += 0.1f;
                }
            }
            else if (markerID == 10)
            {
                if (m_engine.getWindowContext().getSelectedOperation() == "Move")
                {
                    gengPos.x += 0.05f;
                }
                else if (m_engine.getWindowContext().getSelectedOperation() == "Scale")
                {
                    gengScale += 0.1f;
                }
            }
        });

    engine.getWindowContext().addKeyboardPressCallback(GLFW_KEY_RIGHT, [this]
        {
            const auto markerID = m_engine.getWindowContext().getSelectedMarker();

            if (markerID == 120)
            {
                if (m_engine.getWindowContext().getSelectedOperation() == "Move")
                {
                    sylvPos.x -= 0.05f;
                }
                else if (m_engine.getWindowContext().getSelectedOperation() == "Scale")
                {
                    sylvScale -= 0.1f;
                }
            }
            else if (markerID == 10)
            {
                if (m_engine.getWindowContext().getSelectedOperation() == "Move")
                {
                    gengPos.x -= 0.05f;
                }
                else if (m_engine.getWindowContext().getSelectedOperation() == "Scale")
                {
                    gengScale -= 0.1f;
                }
            }
        });
}

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

        if (markers[i].markerID == 120)
        {
            model.model = &m_sylv;
            model.rotationAxis = sylvRotAxis;
            model.rotationValueRad = sylvRotation;
            model.scale = sylvScale;
            model.position = sylvPos;
        }
        else if (markers[i].markerID == 18)
        {
            model.model = &m_mercy;
            model.scale = gengScale;
            model.position = gengPos;
        }
        else
        {
            model.model = &m_mercy;
            model.scale = { 0.1f, 0.1f, 0.1f };
        }

        renderer.getAnyModelRenderComponent().renderModels(std::move(model));
    }
}

 
EndNamespaceOlympus
