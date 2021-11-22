#pragma once

#include <filesystem>

#include <assimp/scene.h>

#include <utils/macros.h>
#include <Texture.h>
#include <Mesh.h>
#include <ShaderProgram.h>

BeginNamespaceOlympus

class Model
{
public:
    //----------------------
    //constructor expects the filepath to
    // 3d model
    Model(const std::filesystem::path& path, bool gamma = false);

    void draw(ShaderProgram& shader) const;

private:
    std::vector<Texture> m_textures;
    std::vector<Mesh> m_meshes;
    std::filesystem::path m_directory;
    bool m_gamma_correction;
    

    // loads a model with supported ASSIMP extensions from file
    // and stores the resulting meshes in the meshes vector.
    void loadModel(const std::filesystem::path& path);

    // processes a node in a recursive fashion. Processes each
    // individual mesh located at the node and repeats this
    // process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name);
};

EndNamespaceOlympus
