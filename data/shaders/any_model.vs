#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 tex_coords;
out vec3 frag_pos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    tex_coords = aTexCoords;
    normal = aNormal;
    frag_pos = vec3(model * vec4(aPos, 1));
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
