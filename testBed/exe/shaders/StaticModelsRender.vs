#version 330 core

//------------------------------------------

layout ( location = 0 ) in vec3 in_Position;
layout ( location = 1 ) in vec3 in_Normal;
layout ( location = 2 ) in vec2 in_TexCoord;

//------------------------------------------

out vec2 TexCoord;

//------------------------------------------

uniform mat4 PVTMatrix;
uniform mat4 TransformMatrix;

//------------------------------------------

void main()
{	
	gl_Position = PVTMatrix * vec4( in_Position.x, in_Position.y, -in_Position.z, 1.0f );		
	TexCoord = in_TexCoord;
}