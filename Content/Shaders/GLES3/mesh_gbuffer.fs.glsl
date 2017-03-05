#version 300 es
precision highp float;

#if _AlbedoMap
	uniform sampler2D AlbedoTexture;
#endif

#if _UV
	in vec2 UVCoord;
#endif

#if _Normals
	in vec3 NormalVS;
#endif

// We only need this code, if we have Normals, UVs, Tangents and a normal map.
// The normalmap-ifdef requires all of the above.
#if _NormalMap 
	in mat3 TBNMatrix;
	uniform sampler2D NormalMap;
#endif

in vec3 VertexPositionVS;
out vec4[3] FragColor;

uniform vec3 Color;

void main() {

	FragColor[0] = vec4(VertexPositionVS, 1.0);

	#if _NormalMap		
		vec3 normal = normalize(TBNMatrix * ( texture(NormalMap, UVCoord).rgb * 2.0 - 1.0));
		FragColor[1] = vec4(normal, 1.0);
	#else
		#if _Normals
			FragColor[1] = vec4(NormalVS, 1);
		#else
			FragColor[1] = vec4(0, 0, 0, 1);
		#endif
	#endif

	#if _AlbedoMap
		FragColor[2] = texture(AlbedoTexture, UVCoord);
	#else
		FragColor[2] = vec4(0, 0, 0, 0);
	#endif
}