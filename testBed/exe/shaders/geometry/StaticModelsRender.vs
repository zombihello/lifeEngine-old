#version 330 core

//------------------------------------------

layout ( location = 0 ) in vec3 in_Position;
layout ( location = 2 ) in vec2 in_TexCoord;

//------------------------------------------

out vec2 TexCoord;

//------------------------------------------

uniform mat4 PVTMatrix;

//------------------------------------------

void main()
{	
	TexCoord = in_TexCoord;	
	gl_Position = PVTMatrix * vec4( in_Position, 1.0f );
}