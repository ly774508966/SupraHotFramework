#version 400

in vec3 VertexPosition;
out vec3 VertexPositionVS;

#if _Normals
	uniform mat3 NormalMatrix;
	in vec3 VertexNormal;
	out vec3 NormalVS;
#endif

#if _UV
	in vec2 VertexUV;
	out vec2 UVCoord;
#endif

#if _NormalMap
	in vec3 VertexTangent;
	in vec3 VertexBiTangent;
	out mat3 TBNMatrix;
#endif

uniform mat4 ModelViewProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ModelMatrix;

void main(){
	gl_Position = ModelViewProjectionMatrix * vec4(VertexPosition, 1);
	VertexPositionVS = (ModelViewMatrix * vec4(VertexPosition, 1)).xyz;

	#if _UV
		UVCoord = VertexUV;
	#endif

	#if _Normals
		NormalVS = normalize(NormalMatrix * VertexNormal);
	#endif

	#if _NormalMap
		vec3 n = NormalVS;
		vec3 t = normalize(NormalMatrix * VertexTangent);
		vec3 bn = normalize(NormalMatrix * VertexBiTangent);
		TBNMatrix = mat3(t, bn, n);
	#endif
}