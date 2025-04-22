#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vertexColor;

uniform float u_aspectRatio;

out vec3 ourColor;
void main()
{
	gl_Position = vec4(aPos.x / u_aspectRatio,  aPos.y, aPos.z, 1.0);
	ourColor = vertexColor;
}
