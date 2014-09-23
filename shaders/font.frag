#version 120

uniform sampler2D tex;
	
void main()
{
	vec4 texel;
	texel = texture2D(tex,gl_TexCoord[0].st);
	gl_FragColor = vec4(gl_Color.rgb, texel.a);
}
