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

    m_format = format;

    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

EndNamespaceOlympus
