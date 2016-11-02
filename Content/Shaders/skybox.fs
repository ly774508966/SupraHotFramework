#version 400

in vec3 UntransformedVertexPosition;
uniform samplerCube CubeMap;

out vec4 FragColor;

void main(){
	vec3 r = UntransformedVertexPosition;
	
	vec3 tex = texture(CubeMap, r).rgb;
	vec3 p3 = vec3(1.0 / 2.2);
	FragColor = vec4(pow(tex, p3), 1);
}