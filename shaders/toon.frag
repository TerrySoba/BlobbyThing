// simple toon fragment shader
// www.lighthouse3d.com

varying vec3 normal, lightDir;
uniform sampler2D tex;

void main()
{
	float intensity;
	vec3 n;
	float factor;

	vec4 texColor = texture2D(tex, gl_TexCoord[0].st);

	n = normalize(normal);
	
	intensity = max(dot(lightDir,n),0.0); 

	if (intensity > 0.98)
		factor = 0.8;
	else if (intensity > 0.5)
		factor = 0.4;
	else if (intensity > 0.25)
		factor = 0.2;
	else
		factor = 0.1;


	if (intensity < 0.05) intensity = 0.05;

	gl_FragColor = vec4(texColor.rgb * intensity, texColor.a);
}
