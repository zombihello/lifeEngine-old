#version 330 core

in vec3 WorldPos;
in vec3 TexCoord;
in vec3 Normal;

layout (location = 0) out vec3 worldPos;
layout (location = 1) out vec3 diffuse;
layout (location = 2) out vec3 normal;

uniform sampler2D ColorMap;

void main()
{		
	worldPos = WorldPos;
	diffuse = texture( ColorMap, TexCoord ).xyz;
	normal = normalize( Normal ) ;
	
	//worldPos = WorldPos;
	//Diffuse = texture( gColorMap, TexCoord ).xyz;
	//normal = normalize( Normal );
	//texCoord = vec3( TexCoord, 0.0f );
}