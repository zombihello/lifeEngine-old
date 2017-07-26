#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 texcoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformationMatrix;

out vec3 WorldPos;
out vec3 TexCoord;
out vec3 Normal;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * transformationMatrix * vec4( position, 1.0f );
	
	WorldPos = ( transformationMatrix * vec4( position, 1.0f ) ).xyz;
	Normal = ( transformationMatrix * vec4( normal, 0.0f ) ).xyz;
	TexCoord = texcoord;
}