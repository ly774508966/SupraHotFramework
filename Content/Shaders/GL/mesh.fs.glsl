#version 400

#if _AlbedoMap
	uniform sampler2D AlbedoTexture;
#endif

in vec3 VertexPositionVS;
out vec4 FragColor;

#if _UV
	in vec2 UVCoord;
#endif

#if _Normals
	in vec3 NormalVS;
#endif

#if _TangentsBiTangents
	in mat3 TangentToViewMatrix;
#endif

uniform vec3 Color;

void main() {
	#if _UV
		FragColor = vec4(UVCoord.xy, 0.0, 1.0);
	#else
		#if _Normals
			FragColor = vec4(NormalVS, 1);
		#else
			FragColor = vec4(VertexPositionVS, 1);
		#endif
	#endif
}