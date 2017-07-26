#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 texcoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformationMatrix;

out vec2 TexCoord;
out vec3 Normal;
out vec3 WorldPos;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * transformationMatrix * vec4( position, 1.0f );
	TexCoord = texcoord;
	Normal = ( transformationMatrix * vec4( normal, 0.0f ) ).xyz;
	WorldPos = ( transformationMatrix * vec4( position, 1.0f ) ).xyz;
}