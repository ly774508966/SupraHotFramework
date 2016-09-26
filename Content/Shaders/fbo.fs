#version 150 core

uniform sampler2D in_texture;
uniform vec2 pixelsize;

in vec2 outUV;
out vec4 FragColor;

void main()
{
	vec2 pos = vec2(gl_FragCoord) * pixelsize;
	FragColor = texture(in_texture, pos);
	//FragColor = vec4(outUV.rg, 0, 1);
}
