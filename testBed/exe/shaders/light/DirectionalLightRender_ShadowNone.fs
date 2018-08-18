#version 330 core

//------------------------------------------

struct DirectionalLight
{
	vec4 Position;
	vec4 Color;
	vec4 Specular;
	
	float Intensivity;
};

//-------------------------------------------

out vec4 Color;

//-------------------------------------------

uniform vec2 ScreenSize;
uniform vec3 ViewPosition;
uniform DirectionalLight light;

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
	vec3 ViewDirection = normalize( ViewPosition - PositionFragment.xyz );
	vec3 lightDirection = normalize( light.Position.xyz );
	vec3 HalfwayDirection = normalize( lightDirection + ViewDirection );
	
	float DiffuseFactor = max( dot( lightDirection, Normal ), 0.0f );	
	float SpecularFactor  = pow( max( dot( Normal, HalfwayDirection ), 0.0 ), 32.0 );
	
	Color = ( light.Color * DiffuseFactor * light.Intensivity + light.Specular * SpecularFactor ) * texture( ColorMap, texCoord );
}