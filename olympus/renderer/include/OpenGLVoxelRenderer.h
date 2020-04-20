#pragma once

#include <filesystem>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <ShaderProgram.h>
#include "DrawCall.h"


namespace rdr
{
    class OpenGLVoxelRenderer
    {
    public:
        OpenGLVoxelRenderer();
        void setClearColor(const glm::vec4& rgbaColor);

        void setCameraPosition(const glm::vec3& camPos) { m_cameraPos = camPos; }
        glm::vec3 getCameraPosition() const { return m_cameraPos; }

        void setRenderField(const glm::vec2& renderFieldPx) { m_renderField = renderFieldPx; }
        glm::vec2 getRenderField() const { return m_renderField; }

        void setCameraAngle(float camAngleDeg) { m_cameraAngleDeg = camAngleDeg; }
        float getCameraAngle() const { return m_cameraAngleDeg; }

        void setNearDistance(float nearDistance) { m_nearDistance = nearDistance; }
        float getNearDistance() const { return m_nearDistance; }

        void setFarDistance(float farDistance) { m_farDistance = farDistance; }
        float getFarDistance() const { return m_farDistance; }

        void setDebugRender(bool debugRender) { m_debugRender = debugRender; }

        void renderVoxels(std::vector<VoxelDrawCall>& voxels);

    private:
        void debugRenderVoxel();

        GLuint m_voxelVBO;
        GLuint m_voxelVAO;
        ShaderProgram m_voxelShader;

        // Debug
        bool m_debugRender;
        ShaderProgram m_debugShader;

        // Camera properties
        glm::vec3 m_cameraPos;
        glm::vec2 m_renderField;
        float m_cameraAngleDeg;
        float m_nearDistance;
        float m_farDistance;
    };
}
