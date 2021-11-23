#include "Model.h"
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include <utils/olyerror.h>

#include <Mesh.h>

BeginNamespaceOlympus

Model::Model(const std::filesystem::path& path, bool gamma)
    : m_gamma_correction(gamma)
{
    loadModel(path);
}

void Model::draw(ShaderProgram& shader) const
{
    for (const Mesh& mesh : m_meshes)
    {
        mesh.draw(shader);
    }
}

void Model::loadModel(const std::filesystem::path& path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);

    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        olyError("Error while loading model '{}'. Error string: {}", path.string(), std::string_view(importer.GetErrorString()));
        return;
    }

    // retrieve the directory path of the filepath
    m_directory = path.parent_path();

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process each mesh located at the current node
    for (unsigned i = 0; i < node->mNumMeshes; ++i)
    {
        // the node object only contains indices to index
        // the actual objects in the scene. 
        // the scene contains all the data, node is just
        // to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }

    // after we've processed all of the meshes (if any) we then
    // recursively process each of the children nodes
    for (unsigned i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;
    std::vector<const Texture*> textures;

    // Walk through each of the mesh's vertices
    for (unsigned i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;

        vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

        vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

        if (mesh->mTextureCoords[0])
        {
            vertex.texture_coords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
        }
        else
        {
            vertex.texture_coords = { 0.0f, 0.0f };
        }

        vertex.tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };

        vertex.bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };

        vertices.push_back(std::move(vertex));
    }

    // now walk through each of the mesh's faces (a face is a mesh
    // its triangle) and retrieve the corresponding vertex indices.
    for (unsigned i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN
    // all this samplers are members of struct Material material.

    // 1. diffuse maps
    std::vector<const Texture*> diffuse_maps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuse_maps.cbegin(), diffuse_maps.cend());

    // 2. specular maps
    std::vector<const Texture*> specular_maps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specular_maps.cbegin(), specular_maps.cend());

    // 3. normal maps
    std::vector<const Texture*> normal_maps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normal_maps.cbegin(), normal_maps.cend());

    // 4. height maps
    std::vector<const Texture*> height_maps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), height_maps.cbegin(), height_maps.cend());

    // return a mesh object created from the extracted mesh data
    return Mesh(std::move(vertices), std::move(indices), std::move(textures));
}

std::vector<const Texture*> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name)
{
    std::vector<const Texture*> textures;

    for (unsigned i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        // check if texture was loaded before and if so, continue to next iteration:
        // skip loading a new texture
        bool skip = false;

        for (const auto& texture : m_textures)
        {
            if (texture->getPath().filename().string() == str.C_Str())
            {
                textures.push_back(texture.get());

                // a texture with the same filepath has already been loaded,
                // continue to next one. (optimization)
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            // if texture hasn't been loaded already, load it
            auto texture = std::make_unique<Texture>(m_directory / str.C_Str());
            texture->setTexTypeByName(type_name);
            textures.push_back(texture.get());
            m_textures.emplace_back(std::move(texture));
        }
    }

    return textures;
}

EndNamespaceOlympus
