#version 400

in vec3 TransformedVertexPosition;
in vec2 UVCoord;

uniform sampler2D AlbedoTexture;

out vec4 FragColor;

void main() {

	// FragColor = vec4(TransformedVertexPosition, 1);
	FragColor = texture(AlbedoTexture, UVCoord);

	//FragColor = vec4(UVCoord.xy, 0, 1);
}