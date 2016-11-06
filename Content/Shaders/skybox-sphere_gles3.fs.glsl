#version 300 es
#define PI 3.14159265358979323
#define INV_PI 0.31830988618379067239521257108191

in highp vec3 UntransformedVertexPosition;
uniform sampler2D SphereMap;

out vec4 FragColor;

vec2 texPanoramaCoords(vec3 dir){ 
    float n = length(dir.xz);
    vec2 pos = vec2( (n>0.0000001) ? max(-1.0,dir.x / n) : 0.0, dir.y);
    if ( pos.x > 0.0 ) pos.x = min( 0.999999, pos.x );
    pos = acos(pos)*INV_PI;
    pos.x = (dir.z > 0.0) ? pos.x*0.5 : 1.0-(pos.x*0.5);
    pos.x = mod((pos.x+0.25+1.0) , 1.0);
    pos.y = 1.0-pos.y; 
    return pos;
}

void main(){
	vec3 r = normalize(UntransformedVertexPosition);
    vec2 uvCoords = texPanoramaCoords(r);
	vec4 tex = texture(SphereMap, uvCoords);
    
	vec3 p3 = vec3(1.0 / 2.2);
	FragColor = vec4(pow(tex.rgb, p3), 1);
}