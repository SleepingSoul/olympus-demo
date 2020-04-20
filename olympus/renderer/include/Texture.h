#pragma once

#include <filesystem>
#include <array>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace rdr
{
    class CubeMap
    {
    public:
        using TextureID = GLuint;

        struct TextureData
        {
            GLsizei width{ 0 };
            GLsizei height{ 0 };
            int numChannels{ 0 };
        };

        CubeMap(const std::filesystem::path& cubeMapTextures);

        auto getID() const { return m_id; }

    private:
        TextureID m_id{ 0 };
        std::array<TextureData, 6> m_cubeMapTextures;

        std::filesystem::path m_path;
    };
}
