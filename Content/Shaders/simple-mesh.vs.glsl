#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;
in vec2 VertexUV;

uniform mat3 NormalMatrix;
uniform mat4 ModelViewProjectionMatrix;
uniform mat4 ModelViewMatrix;

out vec3 TransformedVertexPosition;
out vec2 UVCoord;

void main(){
	vec4 transformedVertex = ModelViewProjectionMatrix * vec4(VertexPosition, 1);
	gl_Position = transformedVertex;

	TransformedVertexPosition = normalize(NormalMatrix * VertexNormal);
	UVCoord = VertexUV;
}