#version 330 core

#define MAX_CUBES 2000

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec3 Diffuse;

uniform mat4 view;
uniform mat4 projection;

layout (std140) uniform BatchBlock
{
	mat4 models[MAX_CUBES];
	vec4 diffuses[MAX_CUBES];
};

void main()
{
	mat4 model = models[gl_InstanceID];
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = aNormal;
	TexCoord = aTexCoord;
	Diffuse = diffuses[gl_InstanceID].xyz;
}
