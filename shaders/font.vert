#version 120

uniform mat4 modelView;                 // shader modelview matrix uniform
uniform mat4 projection;                // shader projection matrix uniform

void main()
{	
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
        // gl_Position = ftransform();
        gl_Position = (gl_ProjectionMatrix + projection) * (gl_ModelViewMatrix + modelView) * gl_Vertex;
}
