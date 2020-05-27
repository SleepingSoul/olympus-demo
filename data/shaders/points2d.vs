#version 330 core

#define MAX_POINTS 100

out vec2 PointPos;
out float PointSize;
out vec4 PointColor;

uniform vec2 positions[MAX_POINTS];
uniform float sizes[MAX_POINTS];
uniform vec4 colors[MAX_POINTS];

uniform ivec2 FramebufferSize;

void main()
{
    float pointSize = sizes[gl_InstanceID];

    gl_PointSize = pointSize;
    gl_Position = vec4(positions[gl_InstanceID], 0.0, 1.0);
    PointPos = FramebufferSize * (gl_Position.xy * 0.5 + 0.5);
    PointSize = pointSize;
    PointColor = colors[gl_InstanceID];
}
