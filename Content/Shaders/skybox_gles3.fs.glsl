#version 300 es

in highp vec3 UntransformedVertexPosition;
uniform samplerCube CubeMap;

out vec4 FragColor;

void main(){
	vec3 r = normalize(UntransformedVertexPosition);
	vec4 tex = texture(CubeMap, r);
	vec3 p3 = vec3(1.0 / 2.2);
	FragColor = vec4(pow(tex.rgb, p3), 1);
}