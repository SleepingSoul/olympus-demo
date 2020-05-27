#version 330 core

in vec2 PointPos;
in float PointSize;
in vec4 PointColor;

out vec4 FragColor;

void main()
{
    float dist = distance(PointPos, gl_FragCoord.xy);

    if (dist > PointSize / 2.0)
    {
        discard;
    }

    FragColor = PointColor;
}
