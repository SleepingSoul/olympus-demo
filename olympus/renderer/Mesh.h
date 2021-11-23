#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <utils/macros.h>
#include <ShaderProgram.h>
#include <Texture.h>

BeginNamespaceOlympus

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_coords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> init_vertices, std::vector<unsigned> init_indices, std::vector<const Texture*> init_textures);

    // render the mesh
    void draw(ShaderProgram& shader) const;

private:
    GLuint m_VBO;
    GLuint m_EBO;
    GLuint m_VAO;

    std::vector<Vertex> m_vertices;
    std::vector<unsigned> m_indices;
    std::vector<const Texture*> m_textures;

    void setupMesh();
};

EndNamespaceOlympus
