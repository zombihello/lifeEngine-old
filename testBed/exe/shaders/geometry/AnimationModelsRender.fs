#version 330 core

//-------------------------------------------

layout ( location = 0 ) out vec3 out_Position;
layout ( location = 1 ) out vec3 out_Diffuse;
layout ( location = 2 ) out vec3 out_Normal;
layout ( location = 3 ) out vec3 out_Lightmap;

//------------------------------------------

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

//------------------------------------------

uniform vec2 ScreenSize;

// Textures
uniform sampler2D ColorMap;

//------------------------------------------

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / ScreenSize;
}

//------------------------------------------

void main()
{
	out_Position = Position;
	out_Diffuse = texture( ColorMap, TexCoord ).xyz;
	out_Normal = normalize( Normal );
	out_Lightmap = vec3( 0 ); // Лайтмапы на динкамику пока что не налаживаются
}