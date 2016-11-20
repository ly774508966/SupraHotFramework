#version 400

in vec3 TransformedVertexPosition;
in vec2 UVCoord;

uniform sampler2D AlbedoTexture;

out vec4 FragColor;

void main() {
	#if _hasCustomValue
		FragColor = vec4(UVCoord.xy, 0, 1);
	#else
		FragColor = texture(AlbedoTexture, UVCoord);
	#endif
}