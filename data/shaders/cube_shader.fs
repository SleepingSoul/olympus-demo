#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 Diffuse;

uniform sampler2D faceTexture;

void main()
{
	vec4 texColor = texture(faceTexture, TexCoord);

	if (texColor.a < 0.1)
	{
        discard;
	}

	vec3 lightColor = vec3(1.0, 0.9, 0.9);
	float ambientStrength = 0.5;
	vec3 ambient = ambientStrength * lightColor;

	vec3 lightPos = vec3(0.0, 2.0, 8.0);
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffusePart = Diffuse * diff * lightColor;

	FragColor = vec4(ambient + diffusePart, 1.0) * texColor;
}
