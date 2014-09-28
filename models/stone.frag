#version 120
varying vec3 normal, eyeVec, lightDir;
uniform sampler2D tex;

void main (void)
{
    gl_FragColor = texture2D(tex, gl_TexCoord[0].st);
}
