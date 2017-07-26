#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos;

uniform sampler2D gColorMap;

layout (location = 0) out vec3 worldPos;
layout (location = 1) out vec3 Diffuse;
layout (location = 2) out vec3 normal;

void main()
{	
	worldPos = WorldPos;
	Diffuse = texture( gColorMap, TexCoord );
	normal = normalize( Normal );
}