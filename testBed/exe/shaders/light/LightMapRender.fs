#version 330 core

//-------------------------------------------

out vec4 Color;

//-------------------------------------------

uniform vec2 ScreenSize;

// Textures
uniform sampler2D ColorMap;
uniform sampler2D LightMap;

//------------------------------------------

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / ScreenSize;
}

//------------------------------------------

void main()
{
	vec2 TexCoord = CalcTexCoord();
	Color = texture( ColorMap, TexCoord ) * texture( LightMap, TexCoord );
}