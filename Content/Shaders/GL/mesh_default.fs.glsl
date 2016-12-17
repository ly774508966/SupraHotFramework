#version 400

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
uniform vec3 DirLight;
uniform mat4 ViewMatrix;

uniform bool TestBool;
uniform float TestValue;
uniform vec2 TestVec2;
uniform vec3 TestVec3;
uniform vec4 TestVec4;

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