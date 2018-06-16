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
in vec2 TexCoord_LightMap;

//------------------------------------------

uniform sampler2D ColorMap;
uniform sampler2D LightMap;

//------------------------------------------

void main()
{
	out_Position = Position;
	out_Diffuse = texture( ColorMap, TexCoord ).xyz;
	out_Normal = normalize( Normal );
	out_Lightmap = texture( LightMap, TexCoord_LightMap ).xyz;
}