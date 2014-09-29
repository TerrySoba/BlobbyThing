#version 330 core

in vec2 uv;
uniform sampler2D tex;

//void main (void)
//{
//    color = texture2D(tex, uv);
//}

in vec3 normalInterp;
in vec3 vertPos;
out vec4 color;

const vec3 lightPos = vec3(1.0,1.0,1.0);
// const vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
const vec3 specColor = vec3(1.0, 1.0, 1.0);

void main() {

  vec3 diffuseColor = texture2D(tex, uv).rgb;
  float alpha = texture2D(tex, uv).a;

  vec3 normal = normalize(normalInterp);
  vec3 lightDir = normalize(lightPos - vertPos);

  float lambertian = max(dot(lightDir,normal), 0.0);
  float specular = 0.0;

  if(lambertian > 0.0) {

    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(-vertPos);

    float specAngle = max(dot(reflectDir, viewDir), 0.0);
    specular = pow(specAngle, 4.0);

    // the exponent controls the shininess
    specular = pow(specAngle, 16.0);
  }

  color = vec4( lambertian*diffuseColor +
                specular*specColor, alpha);
}

