#version 400

in vec3 VertexPosition;
out vec3 TransformedVertexPosition;

uniform mat4 ModelViewProjectionMatrix;

void main(){
	vec4 transformedVertex = ModelViewProjectionMatrix * vec4(VertexPosition, 1);
	gl_Position = transformedVertex;
	TransformedVertexPosition = transformedVertex.xyz;
}