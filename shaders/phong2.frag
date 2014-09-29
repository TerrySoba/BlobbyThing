#version 330 core

uniform sampler2D tex;

in vec2 uv;
in vec3 normalInterp;
in vec3 eyeVec;
in vec3 lightDir;

out vec4 color;

const float materialEmission = 0.0;
const float lightAmbient = 0.2;
const float lightDiffuse = 1;
const float materialShininess = 100;
const float lightSpecular = 1;
const float materialSpecular = 1;

void main (void)
{
  vec4 texColor = texture2D(tex, uv);
  float alpha = texColor.a;
  float ambient_factor = 0.9;
  vec4 final_color = materialEmission + (texColor * ambient_factor) * lightAmbient;
  vec3 N = normalize(normalInterp);
  vec3 L = normalize(lightDir);
  float lambertTerm = dot(N,L);
  if (lambertTerm > 0.0)
  {
    final_color +=
      lightDiffuse *
      texColor *
      lambertTerm;
    vec3 E = normalize(eyeVec);
    vec3 R = reflect(-L, N);
    float specular = pow(max(dot(R, E), 0.0),
                         materialShininess);
    final_color +=
      lightSpecular *
      materialSpecular *
      specular;
    alpha += specular;
  }
  color = vec4(final_color.rgb, alpha);
}





