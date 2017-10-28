#version 330 core

//------------------------------------------

struct SpotLight
{
	vec4 Position;
	vec4 Color;
	vec3 SpotDirection;
	
	float Radius;
	float SpotCutoff;
	float SpotExponent;
};

//-------------------------------------------

out vec4 Color;

//-------------------------------------------

uniform vec2 ScreenSize;
uniform SpotLight light;

// Textures
uniform sampler2D ColorMap;
uniform sampler2D PositionMap;
uniform sampler2D NormalMap;

//------------------------------------------

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / ScreenSize;
}

//------------------------------------------

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 Normal = normalize( texture( NormalMap, texCoord ).xyz );
	vec3 lightDirection = ( light.Position - texture( PositionMap, texCoord ) ).xyz;
	float Distance = length( lightDirection );
	lightDirection = normalize( lightDirection );
	
	float SpotEffect = dot( normalize( light.SpotDirection ), -lightDirection );
	float Spot = float( SpotEffect > light.SpotCutoff );
	
	SpotEffect = max( pow( SpotEffect, light.SpotExponent ), 0.0f );
	
	float DiffuseFactor = max( dot( lightDirection, Normal ), 0.0f );
	float Attenuation =  Spot * SpotEffect;
		
	Color = ( light.Color * DiffuseFactor ) * Attenuation * texture( ColorMap, texCoord );
}