#include "TextureStorage.h"

#include <fstream>

#include <extra_std/extra_std.h>

#include <logging/logging.h>

BeginNamespaceOlympus

namespace
{
    const char* const MappingPath = "data/textures/textures_table.json";
}

Texture* TextureStogare::getTexture(TextureID textureID) const
{
    const auto [found, it] = estd::find(m_mapping, textureID);

    return found ? it->second.get() : nullptr;
}

void TextureStogare::preloadAllTextures()
{
    m_mapping.clear();

    const auto mapping = loadTexturesMapping();

    if (mapping.empty())
    {
        logging::warning("[TextureStorage] preloadAllTextures: textures mapping is invalid.");
        return;
    }

    for (const auto& kv : mapping.items())
    {
        logging::info("[TextureStorage] Loading texture '{}'.", kv.value());
        m_mapping.emplace(static_cast<TextureID>(std::stoi(kv.key())), std::make_unique<Texture>(kv.value().get<std::string>()));
    }
}

nlohmann::json TextureStogare::loadTexturesMapping()
{
    nlohmann::json mapping;

    std::ifstream file(MappingPath);

    file >> mapping;

    return mapping;
}

EndNamespaceOlympus

