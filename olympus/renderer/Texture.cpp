#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

#include <utils/olyerror.h>

#include <utils.h>

BeginNamespaceOlympus

namespace
{
    GLenum channelsNumberToFormat(int channelsNumber)
    {
        switch (channelsNumber)
        {
        case 1:
            return GL_RED;
        case 3:
            return GL_RGB;
        case 4:
            return GL_RGBA;
        default:
            return GL_NONE;
        }
    }
}

std::string_view Texture::TexTypeToString(TexType texType)
{
    switch (texType)
    {
    case TexType::DiffuseType:
        return "texture_diffuse";
    case TexType::SpecularType:
        return "texture_specular";
    case TexType::NormalType:
        return "texture_normal";
    case TexType::HeightType:
        return "texture_height";
    default:
        olyError("Undefined tex type: {}", texType);
        return "";
    }
}

Texture::Texture(const std::filesystem::path& textureFile)
{
    glGenTextures(1, &m_id);

    glBindTexture(GL_TEXTURE_2D, m_id);

    stbi_set_flip_vertically_on_load(true);

    int numChannels = 0;

    stbi_uc* const data = stbi_load(textureFile.u8string().c_str(), &m_width, &m_height, &numChannels, 0);

    if (!data)
    {
        olyError("[Texture] texture load failed (path is: '{}')", textureFile);
        return;
    }

    m_format = channelsNumberToFormat(numChannels);

    if (m_format == GL_NONE)
    {
        olyError("[Texture] unrecognized format of the given image: '{}'", textureFile);
        return;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    m_loadSuccess = true;
}

Texture::Texture(Texture&& other)
    : m_id(other.m_id)
    , m_format(other.m_format)
    , m_height(other.m_height)
    , m_width(other.m_width)
    , m_loadSuccess(other.m_loadSuccess)
    , m_path(std::move(other.m_path))
    , m_texType(other.m_texType)
{
    other.m_id = 0;
    other.m_texType = TexType::Undefined;
    other.m_loadSuccess = false;
}

Texture& Texture::operator =(Texture&& other)
{
    m_id = std::exchange(other.m_id, 0);
    m_format = other.m_format;
    m_height = other.m_height;
    m_width = other.m_width;
    m_loadSuccess = std::exchange(other.m_loadSuccess, false);
    m_path = std::move(other.m_path);
    m_texType = std::exchange(other.m_texType, TexType::Undefined);

    return *this;
}

Texture::~Texture()
{
    if (m_loadSuccess)
    {
        glDeleteTextures(1, &m_id);
    }
}

void Texture::hotReset(const void* data, GLsizei width, GLsizei height, GLenum format)
{
    if (width != m_width || height != m_height)
    {
        olyError("[Texture] texture hot reset with different size values is unsupported.");
        return;
    }

    m_format = format;

    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setTexTypeByName(std::string_view name)
{
    if (name == "texture_diffuse")
    {
        m_texType = TexType::DiffuseType;
    }
    else if (name == "texture_specular")
    {
        m_texType = TexType::SpecularType;
    }
    else if (name == "texture_normal")
    {
        m_texType = TexType::NormalType;
    }
    else if (name == "texture_height")
    {
        m_texType = TexType::HeightType;
    }
    else
    {
        olyError("Undefined name for texture: {}", name);
        m_texType = TexType::Undefined;
    }
}

EndNamespaceOlympus
