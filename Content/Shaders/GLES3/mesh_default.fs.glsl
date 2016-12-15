#version 300 es

in highp vec3 VertexPositionVS;
out highp vec4 FragColor;

#if _UV
	in highp vec2 UVCoord;
#endif

#if _Normals
	in highp vec3 NormalVS;
#endif

#if _TangentsBiTangents
	in highp mat3 TangentToViewMatrix;
#endif

//uniform vec3 DirLight;
//uniform mat4 ViewMatrix;

void main() {
	#if _Normals
		//vec3 dirVS = (ViewMatrix * vec4(DirLight, 0)).xyz;
		//float nDotL = clamp(dot(NormalVS, normalize(dirVS)), 0, 1);
		//FragColor = vec4(Color * nDotL, 1.0);
		FragColor = vec4(NormalVS, 1);
	#elif
		FragColor = vec4(VertexPositionVS, 1);
	#endif
}