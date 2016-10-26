#version 400

in vec3 UntransformedVertexPosition;
uniform samplerCube CubeMap;

out vec4 FragColor;

void main(){
	vec3 r = UntransformedVertexPosition;
	FragColor = texture(CubeMap, r);
	//FragColor = vec4(UntransformedVertexPosition, 1);
}