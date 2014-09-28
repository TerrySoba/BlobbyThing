#version 120
#extension GL_ARB_gpu_shader5 : enable

uniform mat4 modelView;                 // shader modelview matrix uniform
uniform mat4 projection;                // shader projection matrix uniform

varying vec3 normal, eyeVec, lightDir;
void main()
{
  // gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
  // normal = gl_NormalMatrix * gl_Normal;

  gl_Position = projection * modelView * gl_Vertex;

  // normal = transpose(inverse(mat3(gl_ModelViewMatrix))) * gl_Normal;

  normal = transpose(inverse(mat3(modelView))) * gl_Normal;

  vec4 vVertex = gl_ModelViewMatrix * gl_Vertex;
  eyeVec = -vVertex.xyz;
  lightDir = vec3(gl_LightSource[0].position.xyz - vVertex.xyz);
  gl_TexCoord[0] = gl_MultiTexCoord0;
}
