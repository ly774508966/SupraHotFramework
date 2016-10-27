#version 300 es

in vec3 VertexPosition;
out highp vec3 UntransformedVertexPosition;

uniform mat4 ViewProjectionMatrix;

void main(){
	gl_Position = ViewProjectionMatrix * vec4(VertexPosition, 1.0);
	UntransformedVertexPosition = VertexPosition;
}