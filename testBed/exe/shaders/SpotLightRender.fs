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

uniform vec2 ScreenSize;
uniform vec4 AmbientLight;
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
	vec4 PositionFragment = texture( PositionMap, texCoord );
	
	vec3 LightDirection = ( light.Position - PositionFragment ).xyz;
	float Distance = length( LightDirection );
	LightDirection = normalize( LightDirection );
	
	float DiffuseFactor = max( dot( LightDirection, Normal ), 0.0f );
	float SpotFactor = dot( light.SpotDirection, -LightDirection );
	SpotFactor = clamp( ( SpotFactor - light.SpotCutoff ) / ( 1.0f - light.SpotCutoff ), 0.0f, 1.0f );
	float Attenuation = max( 1.0f - pow( Distance / light.Height, 2 ), 0.f );
	
	Color = light.Color * DiffuseFactor * light.Intensivity * SpotFactor * Attenuation * texture( ColorMap, texCoord );
}