#version 400

in vec3 VertexPositionVS;
out vec4 FragColor;

#if _UV
	in vec2 UVCoord;
#endif

#if _Normals
	in vec3 NormalVS;
#endif

#if _TangentsBiTangents
	in mat3 TangentToViewMatrix;
#endif

// Exposed to the front end
uniform vec3 Color;
uniform vec3 DirLight;
uniform vec3 LightColor;

#if _DiffuseTexture
	uniform sampler2D DiffuseTexture;
#endif

// Not exposed inside the editor
uniform mat4 ViewMatrix;

void main() {
	#if _DiffuseTexture

		vec3 dirVS = (ViewMatrix * vec4(DirLight, 0)).xyz;
		float nDotL = clamp(dot(NormalVS, normalize(dirVS)), 0, 1);

		vec4 diffuseColor = texture(DiffuseTexture, UVCoord);

		FragColor = vec4(diffuseColor.rgb * nDotL * LightColor, 1.0);
	#else
		#if _Normals
			//vec3 dirVS = (ViewMatrix * vec4(DirLight, 0)).xyz;
			//float nDotL = clamp(dot(NormalVS, normalize(dirVS)), 0, 1);
			//FragColor = vec4(Color * nDotL * LightColor, 1.0);
			
			FragColor = vec4(NormalVS, 1);
		#else
			FragColor = vec4(VertexPositionVS, 1);
		#endif
	#endif
}