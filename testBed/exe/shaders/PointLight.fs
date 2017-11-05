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
		
	float DiffuseFactor = max( dot( lightDirection, Normal ), 0.0f );
	float Attenuation =  1.0f - pow( Distance / light.Radius, 2 );
		
	Color = ( light.Color * DiffuseFactor * light.Intensivity ) * Attenuation * texture( ColorMap, texCoord );
}