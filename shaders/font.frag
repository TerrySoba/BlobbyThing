#version 330 core

in vec2 uv;
uniform sampler2D tex;

out vec4 color;

void main (void)
{
    vec3 textColor = vec3(1.0,1.0,1.0);
    color = vec4(textColor, texture2D(tex, uv).a);
}
