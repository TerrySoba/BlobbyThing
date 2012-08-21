#version 120
varying vec3 normal, eyeVec, lightDir;
uniform sampler2D tex;

void main (void)
{
  vec4 texColor = texture2D(tex, gl_TexCoord[0].st);
  float alpha = texColor.a;
  // vec4 final_color = gl_FrontLightModelProduct.sceneColor;
  float ambient_factor = 0.9;
  vec4 final_color = gl_FrontMaterial.emission + (texColor * ambient_factor) * gl_LightModel.ambient;
  vec3 N = normalize(normal);
  vec3 L = normalize(lightDir);
  float lambertTerm = dot(N,L);
  if (lambertTerm > 0.0)
  {
    final_color +=
      gl_LightSource[0].diffuse *
      texColor *
      lambertTerm;
    vec3 E = normalize(eyeVec);
    vec3 R = reflect(-L, N);
    float specular = pow(max(dot(R, E), 0.0),
                         gl_FrontMaterial.shininess);
    final_color +=
      gl_LightSource[0].specular *
      gl_FrontMaterial.specular *
      specular;
     
    alpha += specular;
  }
  // gl_FragColor = vec4(final_color.rgb, alpha);
    gl_FragColor = vec4(texColor.rgb, alpha);
  
}