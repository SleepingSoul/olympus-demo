#include "ShaderProgram.h"
#include <iterator>


oly::ShaderProgram::ShaderProgram(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath)
{
    if (!std::filesystem::exists(vertexShaderPath) || !std::filesystem::exists(fragmentShaderPath))
    {
        return;
    }

    std::ifstream vertexFile(vertexShaderPath, std::ios::in),
        fragmentFile(fragmentShaderPath, std::ios::in);

    std::string vertexCode{ std::istreambuf_iterator<char>(vertexFile), std::istreambuf_iterator<char>() };
    std::string fragmentCode{ std::istreambuf_iterator<char>(fragmentFile), std::istreambuf_iterator<char>() };

    const GLuint vertexID = glCreateShader(GL_VERTEX_SHADER);
    const GLuint fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar* const vertexCodePtr = vertexCode.c_str();
    glShaderSource(vertexID, 1, &vertexCodePtr, NULL);
    glCompileShader(vertexID);

    validateShader(vertexID);

    const GLchar* const fragmentCodePtr = fragmentCode.c_str();
    glShaderSource(fragmentID, 1, &fragmentCodePtr, NULL);
    glCompileShader(fragmentID);
    
    validateShader(fragmentID);

    m_ID = glCreateProgram();

    glAttachShader(m_ID, vertexID);
    glAttachShader(m_ID, fragmentID);
    glLinkProgram(m_ID);

    validateShaderProgram(m_ID);

    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
}

void oly::ShaderProgram::validateShader(GLuint ID)
{
    GLint success = 0;

    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    if (success == 0)
    {
        ErrorInfoBuffer buffer;
        glGetShaderInfoLog(ID, 1024, NULL, buffer.data());
        writeErrorInfoAndThrow(buffer);
    }
}

void oly::ShaderProgram::validateShaderProgram(GLuint ID)
{
    GLint success = 0;

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (success == 0)
    {
        ErrorInfoBuffer buffer;
        glGetProgramInfoLog(ID, 1024, NULL, buffer.data());
        writeErrorInfoAndThrow(buffer);
    }
}

void oly::ShaderProgram::writeErrorInfoAndThrow(const oly::ShaderProgram::ErrorInfoBuffer& source)
{
    if (source.front() == '\0')
    {
        return;
    }

    if (!m_errorInfo.empty())
    {
        m_errorInfo.back() = '\n';
    }

    std::copy(source.cbegin(), source.cend(), std::back_inserter(m_errorInfo));

    throw ShaderNotCompiled(m_errorInfo);
}
