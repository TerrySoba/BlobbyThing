#version 330 core

in vec4 position;
in vec4 normal;
in vec2 texCoord;


uniform mat4 modelView;                 // shader modelview matrix uniform
uniform mat4 projection;                // shader projection matrix uniform
uniform mat4 normalMat;                 // normal matrix

out vec2 uv;
out vec3 normalInterp;
out vec3 vertPos;

void main(){
    gl_Position = projection * modelView * position;
    vec4 vertPos4 = modelView * position;
    vertPos = vec3(vertPos4) / vertPos4.w;
    normalInterp = vec3(normalMat * normal);
    uv = texCoord;
}


//void main()
//{
//    gl_Position = projection * modelView * position;
//    uv = texCoord;
//}
