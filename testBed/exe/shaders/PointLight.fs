#version 330 core

//------------------------------------------

struct PointLight
{
	vec4 Position;
	vec4 Color;
	
	float Radius;
	float Intensivity;
};

//-------------------------------------------

out vec4 Color;

//-------------------------------------------

uniform int CountLights;
uniform vec2 ScreenSize;
uniform PointLight light[10];

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
	
	vec3 lightDirection;
	float Distance;
	float DiffuseFactor;
	float Attenuation;
	
	for ( int i = 0; i < CountLights; i++ )
	{
		lightDirection = ( light[i].Position - PositionFragment ).xyz;
		Distance = length( lightDirection );
		lightDirection = normalize( lightDirection );
		
		DiffuseFactor = max( dot( lightDirection, Normal ), 0.0f );
		Attenuation =  max( 1.0f - pow( Distance / light[i].Radius, 2 ), 0.f );

		Color += ( light[i].Color * DiffuseFactor * light[i].Intensivity ) * Attenuation; 
	}
	
	Color *= texture( ColorMap, texCoord );
}