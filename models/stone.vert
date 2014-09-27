#version 120

uniform mat4 modelView;                 // shader modelview matrix uniform
uniform mat4 projection;                // shader projection matrix uniform

varying vec3 normal, eyeVec, lightDir;
void main()
{
  gl_Position = (gl_ProjectionMatrix + projection ) * (gl_ModelViewMatrix + modelView) * gl_Vertex;
  normal = gl_NormalMatrix * gl_Normal;
  vec4 vVertex = gl_ModelViewMatrix * gl_Vertex;
  eyeVec = -vVertex.xyz;
  lightDir = vec3(gl_LightSource[0].position.xyz - vVertex.xyz);
  gl_TexCoord[0] = gl_MultiTexCoord0;
}
