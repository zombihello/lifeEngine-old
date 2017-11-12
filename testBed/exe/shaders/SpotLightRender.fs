#version 330 core

//------------------------------------------

struct SpotLight
{
	vec4 Position;
	vec4 Color;
	vec3 SpotDirection;
	
	float Height;
	float SpotCutoff;
	float Intensivity;
};

//-------------------------------------------

out vec4 Color;

//-------------------------------------------

uniform int CountLights;
uniform vec2 ScreenSize;
uniform SpotLight light[10];

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
	vec4 PositionFragment = texture( PositionMap, texCoord );
	
	vec3  LightDirection;
	float Distance;
	float DiffuseFactor;
	float Attenuation;
	float SpotFactor;
	
	for ( int i = 0; i < CountLights; i++ )
	{
		LightDirection = ( light[i].Position - PositionFragment ).xyz;
		Distance = length( LightDirection );
		LightDirection = normalize( LightDirection );
		
		DiffuseFactor = max( dot( LightDirection, Normal ), 0.0f );
		SpotFactor = dot( light[i].SpotDirection, -LightDirection );
		SpotFactor = clamp( ( SpotFactor - light[i].SpotCutoff ) / ( 1.0f - light[i].SpotCutoff ), 0.0f, 1.0f );
		Attenuation = max( 1.0f - pow( Distance / light[i].Height, 2 ), 0.f );
		
		Color += ( light[i].Color * DiffuseFactor * light[i].Intensivity ) * SpotFactor * Attenuation;
	}
	
	Color *= texture( ColorMap, texCoord );
}