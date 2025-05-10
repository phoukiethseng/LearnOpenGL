#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_BertexColor;
layout (location = 2) in vec2 a_TexCoord;

uniform float u_aspectRatio;

out vec3 ourColor;
out vec2 texCoord;
void main()
{
	gl_Position = vec4(a_Pos.x / u_aspectRatio,  a_Pos.y, a_Pos.z, 1.0);
	ourColor = vertexColor;
	texCoord = a_TexCoord;
}
