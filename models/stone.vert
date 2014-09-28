#version 120

uniform mat4 modelView;                 // shader modelview matrix uniform
uniform mat4 projection;                // shader projection matrix uniform

void main()
{
    gl_Position = projection * modelView * gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;
}
