#version 400

in vec3 VertexPosition;
smooth out vec3 UntransformedVertexPosition;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(){
	vec3 vertexPositionVS = VertexPosition * mat3(ViewMatrix);
	gl_Position = ProjectionMatrix * vec4(vertexPositionVS, 1.0);
	UntransformedVertexPosition = VertexPosition;
}