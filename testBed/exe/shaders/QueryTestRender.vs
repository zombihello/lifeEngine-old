#version 330 core

//------------------------------------------

layout ( location = 0 ) in vec3 in_Position;

//------------------------------------------

uniform mat4 PVTMatrix;

//------------------------------------------

void main()
{	
	gl_Position = PVTMatrix * vec4( in_Position, 1.0f );		
}