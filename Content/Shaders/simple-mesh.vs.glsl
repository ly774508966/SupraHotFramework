#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;

uniform mat3 NormalMatrix;
uniform mat4 ModelViewProjectionMatrix;

out vec3 TransformedVertexPosition;

void main(){
	vec4 transformedVertex = ModelViewProjectionMatrix * vec4(VertexPosition, 1);
	gl_Position = transformedVertex;
	TransformedVertexPosition = NormalMatrix * VertexNormal;
}