#version 330 core

//------------------------------------------

in vec4 FragPos;

//------------------------------------------

uniform vec4 LightPosition;
uniform float FarPlane;
uniform bool IsPointLight;

//------------------------------------------

void main()
{
	if ( IsPointLight )
		gl_FragDepth = length( FragPos - LightPosition ) / FarPlane;
	else
		gl_FragDepth = gl_FragCoord.z;
}

//------------------------------------------