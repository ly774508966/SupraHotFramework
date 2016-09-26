#version 150 core

in vec2 vertexpos;
in vec2 uvPos;

out vec2 outUV;
uniform mat4 modelViewProjectionMatrix;

void main()
{
	gl_Position = modelViewProjectionMatrix * vec4(vertexpos, 0, 1);
    outUV = uvPos;
}