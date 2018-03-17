#version 330 core

//------------------------------------------

struct DirectionalLight
{
	vec4 Position;
	vec4 Color;
	
	float Intensivity;
};

//-------------------------------------------

out vec4 Color;

//-------------------------------------------

uniform vec2 ScreenSize;
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
	float DiffuseFactor = max( dot( normalize( light.Position.xyz ), Normal ), 0.0f );	
	
	Color =  light.Color * DiffuseFactor * light.Intensivity * texture( ColorMap, texCoord );
}