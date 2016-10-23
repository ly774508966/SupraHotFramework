#version 400

uniform sampler2D in_texture;
uniform vec2 pixelsize;

in vec2 outUV;
out vec4 FragColor;

void main()
{
	vec2 pos = vec2(gl_FragCoord) * pixelsize;
	FragColor = texture(in_texture, pos);
}
