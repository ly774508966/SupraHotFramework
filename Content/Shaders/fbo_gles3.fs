#version 300 es

uniform sampler2D in_texture;
uniform vec2 pixelsize;

in highp vec2 outUV;
out vec4 FragColor;

void main()
{
	vec2 pos = vec2(gl_FragCoord) * pixelsize;
	FragColor = texture(in_texture, outUV);
}
