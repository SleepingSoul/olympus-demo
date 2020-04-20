//#include "Texture.h"
//
//#pragma warning(push, 1)
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
//#pragma warning(pop)
//
//#include "utils.h"
//
//
//namespace
//{
//    std::array<std::filesystem::path, 6> faces{
//        "right.jpg",
//        "left.jpg",
//        "top.jpg",
//        "bottom.jpg",
//        "front.jpg",
//        "back.jpg"
//    };
//
//    GLenum channelsNumberToFormat(int channelsNumber)
//    {
//        switch (channelsNumber)
//        {
//        case 1:
//            return GL_RED;
//        case 3:
//            return GL_RGB;
//        case 4:
//            return GL_RGBA;
//        default:
//            return GL_NONE;
//        }
//    }
//}
//
//rdr::CubeMap::CubeMap(const std::filesystem::path& cubeMapTextures)
//{
//    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_id);
//
//    for (size_t i = 0; i < 6; ++i)
//    {
//        auto& currentTextureData = m_cubeMapTextures.at(i);
//
//        auto path = faces[i];
//        stbi_uc* const data = stbi_load(path.u8string().c_str(), &currentTextureData.width, &currentTextureData.height,
//            &currentTextureData.numChannels, 0);
//
//        if (assertcheck(!data, "Failed to load texture!"))
//        {
//            continue;
//        }
//
//        const auto format = channelsNumberToFormat(currentTextureData.numChannels);
//
//        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<GLenum>(i), 0, format, currentTextureData.width, currentTextureData.height, 0,
//            format, GL_UNSIGNED_BYTE, data);
//
//        stbi_image_free(data);
//    }, GL_CLAMP_TO_EDGE);
//    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S,
//    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T GL_CLAMP_TO_EDGE);
//    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//}
