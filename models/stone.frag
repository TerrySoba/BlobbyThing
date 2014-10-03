#version 330 core

in vec2 uv;
uniform sampler2D tex;

out vec4 color;

void main (void)
{
    color = texture2D(tex, uv);
}
