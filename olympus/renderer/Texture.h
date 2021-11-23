#pragma once

#include <filesystem>
#include <glad/glad.h>

#include <utils/macros.h>

BeginNamespaceOlympus

class Texture
{
    OlyNonCopyable(Texture)
public:
    using TextureID = GLuint;

    enum class TexType
    {
        Undefined = 0,
        DiffuseType,
        SpecularType,
        NormalType,
        HeightType
    };

    static std::string_view TexTypeToString(TexType texType);

    // Will load the texture from the given file
    Texture(const std::filesystem::path& textureFile, bool flip = false);

    Texture(Texture&& other);
    Texture& operator =(Texture&& other);

    ~Texture();

    TextureID getID() const { return m_id; }
    GLsizei getWidth() const { return m_width; }
    GLsizei getHeight() const { return m_height; }
    GLenum getFormat() const { return m_format; }
    TexType getTexType() const { return m_texType; }
    const std::filesystem::path& getPath() const { return m_path; }

    void setTexTypeByName(std::string_view name);

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

    TexType m_texType{ TexType::Undefined };
};

EndNamespaceOlympus
