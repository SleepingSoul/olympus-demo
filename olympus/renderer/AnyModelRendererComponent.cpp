#include "AnyModelRendererComponent.h"

#include <easy/profiler.h>

#include <utils/olyerror.h>

BeginNamespaceOlympus

namespace
{
    const char* const ModelName = "model";
    const char* const View = "view";
    const char* const Projection = "projection";
}

AnyModelRendererComponent::AnyModelRendererComponent()
    : m_modelShader("data/shaders/any_model.vs", "data/shaders/any_model.fs")
{}

void AnyModelRendererComponent::render(const Camera& camera)
{
    EASY_FUNCTION(profiler::colors::Red);

    glEnable(GL_DEPTH_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glStencilMask(0x00);

    m_modelShader.use();

    for (const auto& worldModel : m_models.backBuffer())
    {
        if (worldModel.modelView.size() != cv::Size(4, 4) || worldModel.modelView.type() != CV_32F || worldModel.projection.size() != cv::Size(4, 4) || worldModel.projection.type() != CV_32F)
        {
            logging::warning("[AnyModelRendererComponent] Cannot perform render operation because input parameters are invalid.");
            return;
        }

        glm::vec3 defaultLight = { 0.5f, 0.5f, 0.5f };

        m_modelShader.setVec3f("dlight.ambient", defaultLight);
        m_modelShader.setVec3f("dlight.diffuse", defaultLight);
        m_modelShader.setVec3f("dlight.specular", defaultLight);
        m_modelShader.setVec3f("dlight.direction", defaultLight);

        if (&worldModel.modelView.at<float>(0)) [[likely]]
        {
            m_modelShader.setMatrix4f(View, &worldModel.modelView.at<float>(0));
        }
        else [[unlikely]]
        {
            logging::error("[AnyModelRendererComponent] Model View matrix is invalid. Skipping updating shader info.");
        }

        glm::mat4 defaultModel = glm::identity<glm::mat4>();
        defaultModel = glm::scale(defaultModel, { 0.2f, 0.2f, 0.2f });
        defaultModel = glm::rotate(defaultModel, glm::radians(90.f), { 1.f, 0.f, 0.f });

        m_modelShader.setMatrix4f(ModelName, &defaultModel[0][0]);

        if (&worldModel.projection.at<float>(0)) [[likely]]
        {
            m_modelShader.setMatrix4f(Projection, &worldModel.projection.at<float>(0));
        }
        else [[unlikely]]
        {
            logging::error("[AnyModelRendererComponent] Projection matrix is invalid. Skipping updating shader info.");
        }

        m_modelShader.setVec3f("viewer_pos", glm::vec3(0.f, 0.f, 0.f));
        m_modelShader.setFloat("material.shininess", 8.f);

        if (worldModel.model)
        {
            worldModel.model->draw(m_modelShader);
        }
        else
        {
            logging::error("[AnyModelRendererComponent] Cannot draw model because it is nullptr.");
        }
    }

    m_modelShader.unuse();

    glDisable(GL_DEPTH_TEST);
}

EndNamespaceOlympus