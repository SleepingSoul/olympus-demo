#pragma once

#include <map>
#include <memory>

#include <nlohmann/json.hpp>

#include <utils/macros.h>

#include <Texture.h>
#include <generated/all_textures.hpp>

BeginNamespaceOlympus

class Texture;

class TextureStogare
{
    OlyNonCopyable(TextureStogare)
public:
    using Mapping = std::map<TextureID, std::unique_ptr<Texture>>;

    TextureStogare() = default;

    Texture* getTexture(TextureID textureID) const;

    void preloadAllTextures();
    
private:
    nlohmann::json loadTexturesMapping();

    Mapping m_mapping;
};

EndNamespaceOlympus
