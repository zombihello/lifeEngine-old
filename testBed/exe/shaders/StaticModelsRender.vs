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

uniform mat4 PVMatrix;
uniform mat4 TransformMatrix;

//------------------------------------------

void main()
{	
	Position = (TransformMatrix * vec4( in_Position.x, in_Position.y, -in_Position.z, 1.0f ) ).xyz;
	Normal = (TransformMatrix * vec4( in_Normal, 0.0f ) ).xyz;
	TexCoord = in_TexCoord;
	
	gl_Position = PVMatrix * vec4( Position, 1.0f );
}