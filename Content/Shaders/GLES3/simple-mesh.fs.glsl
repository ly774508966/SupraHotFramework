#version 300 es

in highp vec3 TransformedVertexPosition;
in highp vec2 UVCoord;

uniform sampler2D AlbedoTexture;

out vec4 FragColor;

void main() {
	FragColor = texture(AlbedoTexture, UVCoord);
}