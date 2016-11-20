#version 300 es
precision highp float;

in vec3 VertexPosition;

#if _Normals
	uniform mat3 NormalMatrix;
	in vec3 VertexNormal;
	out vec3 NormalVS;
#endif

#if _UV
	in vec2 VertexUV;
	out vec2 UVCoord;
#endif

uniform mat4 ModelViewProjectionMatrix;
uniform mat4 ModelViewMatrix;

out vec3 VertexPositionVS;

void main(){
	vec4 transformedVertex = ModelViewProjectionMatrix * vec4(VertexPosition, 1);
	gl_Position = transformedVertex;

	VertexPositionVS = (ModelViewMatrix * vec4(VertexPosition, 1)).xyz;

	#if _UV
		UVCoord = VertexUV;
	#endif

	#if _Normals
		NormalVS = normalize(NormalMatrix * VertexNormal);
	#endif
}