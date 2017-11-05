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
	vec2 TexCoord = CalcTexCoord();
	vec3 Normal = normalize( texture( NormalMap, TexCoord ).xyz );
	Color = texture( ColorMap, TexCoord );
	
	vec3 LightDirection = ( light.Position - texture( PositionMap, TexCoord ) ).xyz;
	float Distance = length( LightDirection );
	LightDirection = normalize( LightDirection );
	
	float DiffuseFactor = max( dot( LightDirection, Normal ), 0.0f );
	Color *= light.Color * DiffuseFactor * light.Intensivity;
	
	float SpotFactor = dot( light.SpotDirection, -LightDirection );
	SpotFactor = clamp( ( SpotFactor - light.SpotCutoff ) / ( 1.0f - light.SpotCutoff ), 0.0f, 1.0f );
	Color *= SpotFactor;
	
	float Attenuation = 1.0f - pow( Distance / light.Height, 2 );
	Color *= Attenuation;
		
	//Color *= 1.0f - ( 1.0f - SpotFactor ) * ( 1.0f / ( 1.0f - light.SpotCutoff ) );	
}