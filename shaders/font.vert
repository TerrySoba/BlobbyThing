#version 330 core

in vec4 position;
in vec2 texCoord;

uniform mat4 modelView;                 // shader modelview matrix uniform
uniform mat4 projection;                // shader projection matrix uniform

out vec2 uv;

void main()
{
    gl_Position = projection * modelView * position;
    uv = texCoord;
}
