#version 330 core

#define MAX_POINTS 100

out vec2 PointPos;
out float PointSize;
out vec4 PointColor;

uniform vec3 positions[MAX_POINTS];
uniform float sizes[MAX_POINTS];
uniform vec4 colors[MAX_POINTS];
uniform mat4 view[MAX_POINTS];

uniform ivec2 FramebufferSize;

void main()
{
    float pointSize = sizes[gl_InstanceID];

    gl_PointSize = pointSize;
    gl_Position = view[gl_InstanceID] * vec4(positions[gl_InstanceID], 1.0);
    PointPos = FramebufferSize * (gl_Position.xy * 0.5 + 0.5);
    PointSize = pointSize;
    PointColor = colors[gl_InstanceID];
}
