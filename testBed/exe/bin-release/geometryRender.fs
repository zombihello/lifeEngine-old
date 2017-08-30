#version 330 core

//-------------------------------------------

out vec4 Color;

//------------------------------------------

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

//------------------------------------------

uniform sampler2D ColorMap;

//------------------------------------------

void main()
{
	Color = texture( ColorMap, TexCoord );
}