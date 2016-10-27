#version 300 es

in highp vec3 UntransformedVertexPosition;
uniform samplerCube CubeMap;

out vec4 FragColor;

void main(){
	vec3 r = normalize(UntransformedVertexPosition);
	FragColor = texture(CubeMap, r);
}