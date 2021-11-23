#include "Mesh.h"

#include <string>

#include <logging/logging.h>

BeginNamespaceOlympus

Mesh::Mesh(std::vector<Vertex> init_vertices, std::vector<unsigned> init_indices, std::vector<const Texture*> init_textures)
    : m_vertices(std::move(init_vertices))
    , m_indices(std::move(init_indices))
    , m_textures(std::move(init_textures))
{
    setupMesh();
}

void Mesh::draw(ShaderProgram& shader) const
{
    // bind appropriate textures
    unsigned diffuse_num = 1;
    unsigned specular_num = 1;
    unsigned normal_num = 1;
    unsigned height_num = 1;

    for (size_t i = 0; i < m_textures.size(); ++i)
    {
        // active proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);

        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        const std::string name{ Texture::TexTypeToString(m_textures.at(i)->getTexType()) };

        if (name == "texture_diffuse")
        {
            number = std::to_string(diffuse_num++);
        }
        else if (name == "texture_specular")
        {
            number = std::to_string(specular_num++);
        }
        else if (name == "texture_normal")
        {
            number = std::to_string(normal_num++);
        }
        else if (name == "texture_height")
        {
            number = std::to_string(height_num++);
        }

        glUniform1i(glGetUniformLocation(shader.getID(), ("material." + name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i]->getID());
    }

    // draw mesh
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texture_coords)));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, bitangent)));

    glBindVertexArray(0);
}

EndNamespaceOlympus
