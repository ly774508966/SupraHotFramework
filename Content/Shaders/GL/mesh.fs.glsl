#version 400

uniform sampler2D AlbedoTexture;
in vec3 VertexPositionVS;
out vec4 FragColor;

#if _UV
	in vec2 UVCoord;
#endif

#if _Normals
	in vec3 NormalVS;
#endif

void main() {
	
	#if _UV
		FragColor = texture(AlbedoTexture, UVCoord);
	#else
		FragColor = vec4(VertexPositionVS, 1);
	#endif

}