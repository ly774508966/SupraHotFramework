#version 400

in vec3 TransformedVertexPosition;

out vec4 FragColor;

void main() {
	FragColor = vec4(TransformedVertexPosition, 1);
}