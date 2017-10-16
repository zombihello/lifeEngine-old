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
	vec4 position = texture( PositionMap, texCoord );
	vec3 color = texture( ColorMap, texCoord ).rgb;
	vec3 Normal = texture( NormalMap, texCoord ).xyz;
	vec3 lightDirection = ( light.Position - position ).xyz;
	float Distance = length( lightDirection );
	lightDirection = normalize( lightDirection );
	Normal = normalize( Normal );
						
	float DiffuseFactor = max( dot( lightDirection, Normal ), 0.0f );
	float Attenuation = 1 - Distance / light.Radius;
		
	vec4 DiffuseColor = ( light.Color * light.Intensivity * DiffuseFactor ) * Attenuation;
		
	Color = DiffuseColor * vec4( color, 1.0f );
}