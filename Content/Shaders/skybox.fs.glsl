#version 400

in vec3 UntransformedVertexPosition;
uniform samplerCube CubeMap;

out vec4 FragColor;

void main(){
	vec3 r = normalize(UntransformedVertexPosition);
	vec3 tex = textureLod(CubeMap, r, 0).rgb;
	vec3 p3 = vec3(1.0 / 2.2);
	FragColor = vec4(pow(tex, p3), 1);
}