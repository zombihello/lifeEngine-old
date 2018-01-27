#version 330 core

//------------------------------------------

in vec4 FragPos;

//------------------------------------------

uniform vec4 LightPosition;
uniform float FarPlane;

//------------------------------------------

void main()
{
	gl_FragDepth = length( FragPos - LightPosition ) / FarPlane;
}

//------------------------------------------