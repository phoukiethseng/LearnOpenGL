#version 330 core

out vec4 FragColor;
in vec3 ourColor;
in vec2 texCoord;

uniform vec4 u_Color;
uniform sampler2D u_BrickWallTexture;

void main()
{
	FragColor = vec4(ourColor, 1.0f) + u_Color + texture(u_BrickWallTexture, texCoord);
}
