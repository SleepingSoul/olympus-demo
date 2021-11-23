#version 330 core

out vec4 frag_color;
in vec2 tex_coords;
in vec3 frag_pos;
in vec3 normal;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct DirLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

uniform Material material;
uniform vec3 viewer_pos;

uniform DirLight dlight;

//-----------------------------------
// function calculates the direcional light component of exact direcional
// light source, using normal parameter for diffuse lighting component
// calculating and view_dir parameter for specular.
vec4 calcDirLight(const DirLight dlight, const vec3 normal, const vec3 view_dir);

void main()
{
    vec3 view_dir = normalize(viewer_pos - frag_pos);
    vec3 norm = normalize(normal);
	vec4 result = calcDirLight(dlight, norm, view_dir);
    if (result.a < 0.1f)
        discard;
	frag_color = result;
}

vec4 calcDirLight(const DirLight dlight, const vec3 normal, const vec3 view_dir)
{
    // ambient component
    vec4 ambient = vec4(dlight.ambient, 1) * texture(material.texture_diffuse1, tex_coords);
    
    // calculate light dir. It is required for diffuse component calculating.
    vec3 light_dir = normalize(-dlight.direction);
    vec4 diffuse = max(dot(light_dir, normal), 0) * vec4(dlight.diffuse, 1) * texture(material.texture_diffuse1, tex_coords);
    
    //calculating reflect direction required for next specular component calc.
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(reflect_dir, view_dir), 0), material.shininess);
    vec4 specular = texture(material.texture_specular1, tex_coords) * spec * vec4(dlight.specular, 1);
    
    return ambient + diffuse + specular;
}
