#version 400

in vec3 VertexPosition;
out vec3 VertexPositionVS;

#if _Normals
	uniform mat3 NormalMatrix;
	in vec3 VertexNormal;
	out vec3 NormalVS;
	out vec3 NormalWS;
#endif

#if _UV
	in vec2 VertexUV;
	out vec2 UVCoord;
#endif

uniform mat4 ModelViewProjectionMatrix;
uniform mat4 ModelViewMatrix;

void main(){
	vec4 transformedVertex = ModelViewProjectionMatrix * vec4(VertexPosition, 1);
	gl_Position = transformedVertex;

	VertexPositionVS = (ModelViewMatrix * vec4(VertexPosition, 1)).xyz;

	#if _UV
		UVCoord = VertexUV;
	#endif

	#if _Normals
		NormalVS = normalize(NormalMatrix * VertexNormal);
		NormalWS = normalize(VertexNormal);
	#endif
}