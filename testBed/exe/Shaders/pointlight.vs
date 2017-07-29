#version 330 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec2 textcoord;

uniform mat4 PVMatrix;
uniform mat4 transformationMatrix;

void main()
{
	gl_Position = PVMatrix * transformationMatrix * vec4( position, 1.0f );
}