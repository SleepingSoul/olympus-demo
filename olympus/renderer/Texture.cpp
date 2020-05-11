#include "Texture.h"

#include <stb_image.h>

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

Texture::Texture(const std::filesystem::path& textureFile)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);

    glBindTexture(GL_TEXTURE_2D, m_id);

    stbi_uc* const data = stbi_load(textureFile.u8string().c_str(), &m_width, &m_height, &m_numChannels, 0);

    if (!data)
    {
        olyError("[Texture] texture load failed (path is: '{}')", textureFile);
        return;
    }

    const auto format = channelsNumberToFormat(m_numChannels);

    if (format == GL_NONE)
    {
        olyError("[Texture] unrecognized format of the given image: '{}'", textureFile);
        return;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    m_loadSuccess = true;
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

void Texture::hotReset(const void* data, GLsizei width, GLsizei height, GLenum format)
{
    if (width != m_width || height != m_height)
    {
        olyError("[Texture] texture hot reset with different size values is unsupported.");
        return;
    }

    glTextureSubImage2D(m_id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
}

EndNamespaceOlympus
