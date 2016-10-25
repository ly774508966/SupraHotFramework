#version 400

in vec3 VertexPosition;
out vec3 UntransformedVertexPosition;

uniform mat4 ViewProjectionMatrix;

void main(){
	gl_Position = ViewProjectionMatrix * vec4(VertexPosition, 1.0);
	UntransformedVertexPosition = VertexPosition;
}