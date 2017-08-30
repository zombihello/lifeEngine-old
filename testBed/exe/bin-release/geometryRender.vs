#version 330 core

//------------------------------------------

layout ( location = 0 ) in vec3 in_Position;
layout ( location = 1 ) in vec3 in_Normal;
layout ( location = 2 ) in vec2 in_TexCoord;

//------------------------------------------

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

//------------------------------------------

uniform mat4 PVTMatrix;
uniform mat4 TransformMatrix;

//------------------------------------------

void main()
{
	Position = ( TransformMatrix * vec4( in_Position, 1.0f ) ).xyz;
	Normal = ( TransformMatrix * vec4( in_Normal, 0.0f ) ).xyz;
		
	gl_Position = PVTMatrix * vec4( in_Position, 1.0f );		
	TexCoord = in_TexCoord;
}