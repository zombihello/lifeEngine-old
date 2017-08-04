#version 330 core

//------------------------------------------

layout ( location = 0 ) in vec3 Position;

//------------------------------------------

uniform mat4 TransformMatrix;

//------------------------------------------

void main()
{
	gl_Position = TransformMatrix * vec4( Position, 1.0f );
}

//------------------------------------------