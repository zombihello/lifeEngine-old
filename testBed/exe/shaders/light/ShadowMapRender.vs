#version 330 core

//------------------------------------------

layout ( location = 0 ) in vec3 Position;

//------------------------------------------

out vec4 FragPos;

//------------------------------------------

uniform mat4 LightMatrices;

//------------------------------------------

void main()
{
	FragPos = vec4( Position, 1.0f );
	gl_Position = LightMatrices * FragPos;
}

//------------------------------------------