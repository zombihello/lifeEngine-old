#version 330 core

//------------------------------------------

struct PointLight
{
	vec4 Position;
	vec4 Color;
	vec4 Specular;
	
	float Radius;
	float Intensivity;
};

//-------------------------------------------

out vec4 Color;

//-------------------------------------------

uniform vec2 ScreenSize;
uniform vec3 ViewPosition;
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

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 Normal = normalize( texture( NormalMap, texCoord ).xyz );	
	vec4 PositionFragment = texture( PositionMap, texCoord );
	vec3 ViewDirection = normalize( ViewPosition - PositionFragment.xyz );
	
	vec3 lightDirection = ( light.Position - PositionFragment ).xyz;
	vec3 HalfwayDirection = normalize( lightDirection + ViewDirection );
	float Distance = length( lightDirection );
	lightDirection = normalize( lightDirection );
		
	float NdotL = dot( Normal, lightDirection );
	float DiffuseFactor = max( NdotL, 0.0f );
	float Attenuation =  max( 1.0f - pow( Distance / light.Radius, 2 ), 0.f );
	float SpecularFactor  = pow( max( dot( Normal, HalfwayDirection ), 0.0 ), 32.0 );	
		
	Color = ( light.Color * DiffuseFactor * light.Intensivity + light.Specular * SpecularFactor ) * Attenuation * texture( ColorMap, texCoord ); 
}