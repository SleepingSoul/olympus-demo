#pragma once

#include <filesystem>
#include <glad/glad.h>

#include <utils/macros.h>

BeginNamespaceOlympus

class Texture
{
public:
    using TextureID = GLuint;

    // Will load the texture from the given file
    Texture(const std::filesystem::path& textureFile);
    ~Texture();

    TextureID getID() const { return m_id; }
    GLsizei getWidth() const { return m_width; }
    GLsizei getHeight() const { return m_height; }
    GLenum getFormat() const { return m_format; }

    bool isValid() const { return m_id != 0 && m_loadSuccess; }

    void hotReset(const void* data, GLsizei width, GLsizei height, GLenum format);

    bool operator <(const Texture& other) const
    {
        return m_id < other.m_id;
    }

    bool operator ==(const Texture& other) const
    {
        return m_id == other.m_id;
    }

    bool operator !=(const Texture& other) const
    {
        return m_id != other.m_id;
    }

private:
    TextureID m_id{ 0 };
    bool m_loadSuccess{ false };

    GLsizei m_width{ 0 };
    GLsizei m_height{ 0 };
    GLenum m_format;

    std::filesystem::path m_path;
};

EndNamespaceOlympus
