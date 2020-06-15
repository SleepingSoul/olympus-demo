#pragma once

#include <filesystem>
#include <string_view>
#include <fstream>
#include <array>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <utils/macros.h>
#include <utils.h>

BeginNamespaceOlympus

DeclareInfoException(ShaderNotCompiled);

class ShaderProgram
{
    OlyNonCopyable(ShaderProgram)
public:
    using ErrorInfoBuffer = std::array<char, 1024>;

    ShaderProgram() = default;

    ShaderProgram(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath);

    struct InitParameters
    {
        std::filesystem::path vsPath;
        std::string vertexUniformBlockName;
        size_t vertexUniformBlockCapacityBytes;
        std::filesystem::path fsPath;
    };

    ShaderProgram(const InitParameters& initParams);

    void use() noexcept { glUseProgram(m_ID); }
    void unuse() noexcept { glUseProgram(0); }

    void setVertexUniformBufferSub(size_t index, const void* data, size_t size)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferID);

        glBufferSubData(GL_UNIFORM_BUFFER, index, size, data);

        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void setBool(const std::string_view& name, bool value) noexcept
    {
        glUniform1i(glGetUniformLocation(m_ID, name.data()), static_cast<int>(value));
    }

    void setInt(const std::string_view& name, int value) noexcept
    {
        glUniform1i(glGetUniformLocation(m_ID, name.data()), value);
    }

    void setFloat(const std::string_view& name, float value) noexcept
    {
        glUniform1f(glGetUniformLocation(m_ID, name.data()), value);
    }

    void setVec2f(const std::string_view& name, const glm::vec2& value)
    {
        glUniform2f(glGetUniformLocation(m_ID, name.data()), value.x, value.y);
    }

    void setVec2i(const std::string_view& name, const glm::ivec2& value)
    {
        glUniform2i(glGetUniformLocation(m_ID, name.data()), value.x, value.y);
    }

    void setVec3f(const std::string_view& name, const glm::vec3& value)
    {
        glUniform3f(glGetUniformLocation(m_ID, name.data()), value.x, value.y, value.z);
    }

    void setVec4f(const std::string_view& name, const glm::vec4& value)
    {
        glUniform4f(glGetUniformLocation(m_ID, name.data()), value.r, value.g, value.b, value.a);
    }

    void setMatrix4f(const std::string_view& name, const float* data) noexcept
    {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.data()), 1, GL_FALSE, data);
    }

    void setMatrix4f(const std::string_view& name, const std::array<float, 4>& floats) noexcept
    {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.data()), 1, GL_FALSE, floats.data());
    }

    bool isValid() const { return m_ID && m_errorInfo.empty(); }
    operator bool() const { return isValid(); }

    const std::string_view& getErrorInfo() const { return m_errorInfo; }

private:
    GLuint m_ID{ 0 };
    GLuint m_uniformBufferID{ 0 };

    std::string m_errorInfo;

    void validateShaderProgram(GLuint ID);
    void validateShader(GLuint ID);
    void writeErrorInfoAndThrow(const ErrorInfoBuffer& source);
};

EndNamespaceOlympus
