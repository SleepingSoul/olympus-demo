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
    , m_model("data/models/sylvanas.obj")
{}

void AnyModelRendererComponent::render(const Camera& camera)
{
    EASY_FUNCTION(profiler::colors::Red);

    glEnable(GL_DEPTH_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    if (m_modelView.size() != cv::Size(4, 4) || m_modelView.type() != CV_32F || m_projection.size() != cv::Size(4, 4) || m_projection.type() != CV_32F)
    {
        logging::warning("[AnyModelRendererComponent] Cannot perform render operation because input parameters are invalid.");
        return;
    }

    m_modelShader.use();

    logging::critical("Modelview: {} {} {} {} {} {} {} {} {}", m_modelView.at<float>(0), m_modelView.at<float>(1), m_modelView.at<float>(2),
        m_modelView.at<float>(3), m_modelView.at<float>(4), m_modelView.at<float>(5),
        m_modelView.at<float>(6), m_modelView.at<float>(7), m_modelView.at<float>(8));

    m_modelShader.setMatrix4f(View, &m_modelView.at<float>(0));

    glm::mat4 defaultModel = glm::identity<glm::mat4>();

    m_modelShader.setMatrix4f(ModelName, &defaultModel[0][0]);
    m_modelShader.setMatrix4f(Projection, &m_projection.at<float>(0));

    m_modelShader.setVec3f("viewer_pos", glm::vec3(0.f, 0.f, 0.f));
    m_modelShader.setFloat("material.shininess", 8.f);

    glStencilMask(0x00);

    m_model.draw(m_modelShader);

    m_modelShader.unuse();

    glDisable(GL_DEPTH_TEST);
}

EndNamespaceOlympus