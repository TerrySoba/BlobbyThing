#version 330 core

in vec4 position;
in vec4 normal;
in vec2 texCoord;

uniform mat4 modelView;                 // shader modelview matrix uniform
uniform mat4 projection;                // shader projection matrix uniform
uniform mat4 normalMat;                 // normal matrix

out vec2 uv;
out vec3 normalInterp;
out vec3 eyeVec;
out vec3 lightDir;

const vec4 lightPosition = vec4(10.0,10.0,20.0,1);

void main()
{
  gl_Position = projection * modelView * position;
  vec4 tmpNormal = normalMat * normal;
  normalInterp = tmpNormal.xyz / tmpNormal.w;
  vec4 vVertex = modelView * position;
  eyeVec = -vVertex.xyz / vVertex.w;
  lightDir = vec3(vec4(modelView * lightPosition).xyz - vVertex.xyz);
  uv = texCoord;
}
