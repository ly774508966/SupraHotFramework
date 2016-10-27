#version 300 es

in vec2 VertexPosition;
in vec2 UVCoord;

out highp vec2 outUV;
uniform mat4 modelViewProjectionMatrix;

void main()
{
	gl_Position = modelViewProjectionMatrix * vec4(VertexPosition, 0, 1);
    outUV = UVCoord;
}