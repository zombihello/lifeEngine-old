#version 330 core

//------------------------------------------

in vec4 FragPos;

//------------------------------------------

uniform vec3 LightPosition;
uniform float Far_Plane;

//------------------------------------------

void main()
{
	gl_FragDepth = length( FragPos.xyz - LightPosition ) / Far_Plane;
}

//------------------------------------------