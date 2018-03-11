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

uniform vec2 ScreenSize;
uniform PointLight light;

// Textures
uniform sampler2D ColorMap;
uniform sampler2D PositionMap;
uniform sampler2D NormalMap;
uniform sampler2D ShadowMap;

//------------------------------------------

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / ScreenSize;
}

//------------------------------------------

//------------------------------------------

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 Normal = normalize( texture( NormalMap, texCoord ).xyz );	
	vec4 PositionFragment = texture( PositionMap, texCoord );
	
	vec3 lightDirection = ( light.Position - PositionFragment ).xyz;
	float Distance = length( lightDirection );
	lightDirection = normalize( lightDirection );
		
	float NdotL = dot( Normal, lightDirection );
	float DiffuseFactor = max( NdotL, 0.0f );
	float Attenuation =  max( 1.0f - pow( Distance / light.Radius, 2 ), 0.f );
		
	Color = light.Color * DiffuseFactor * light.Intensivity * Attenuation * texture( ColorMap, texCoord ); 
}