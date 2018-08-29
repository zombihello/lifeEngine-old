#version 330 core

//-------------------------------------------

out vec4 Color;

//------------------------------------------

in vec2 TexCoord;
in vec2 TexCoord_LightMap;

//------------------------------------------

uniform sampler2D ColorMap;
uniform sampler2D LightMap;

//------------------------------------------

void main()
{
	Color = texture( ColorMap, TexCoord ) * texture( LightMap, TexCoord_LightMap );
}