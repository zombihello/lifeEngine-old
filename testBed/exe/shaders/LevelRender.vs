#version 330 core

//------------------------------------------

layout ( location = 0 ) in vec3 in_Position;
layout ( location = 1 ) in vec3 in_Normal;
layout ( location = 2 ) in vec2 in_TexCoord;

//------------------------------------------

out vec2 TexCoord;

//------------------------------------------

uniform mat4 PVMatrix;

//------------------------------------------

void main()
{	
	gl_Position = PVMatrix * vec4( in_Position, 1.0f );		
	TexCoord = in_TexCoord;
}