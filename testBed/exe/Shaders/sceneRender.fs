#version 330 core

#define GEOMETRY_RENDER 0
#define STENCIL_TEST 1
#define POINT_LIGHT 2

//------------------------------------------

struct PointLight
{
	vec3 Position;
	vec4 Color;
	
	float Radius;
	float Intensivity;
};

//-------------------------------------------

layout ( location = 0 ) out vec3 out_Position;
layout ( location = 1 ) out vec3 out_Diffuse;
layout ( location = 2 ) out vec3 out_Normal;

//------------------------------------------

layout ( location = 0 ) in vec3 Position;
layout ( location = 1 ) in vec3 Normal;
layout ( location = 2 ) in vec2 TexCoord;

//------------------------------------------

uniform int TypeShader;
uniform vec2 ScreenSize;
uniform vec3 ViewPosition;
uniform PointLight light;

// Textures
uniform sampler2D ColorMap;
uniform sampler2D PositionMap;
uniform sampler2D NormalMap;
uniform samplerCube DepthMap;

//------------------------------------------

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / ScreenSize;
}

//------------------------------------------

float ShadowCalculation( vec3 FragPos, vec3 Normal, vec3 LightDirection, PointLight Light )
{
    vec3 FragToLight = FragPos - Light.Position; 
	float CurrentDepth = length( FragToLight );
	
    float ClosestDepth = texture( DepthMap, FragToLight ).r;
    ClosestDepth *= Light.Radius;
	   
    float Bias = max( 0.05f * ( 1.0f - dot( Normal, LightDirection ) ), 0.0005f );   
    float Shadow = CurrentDepth -  Bias > ClosestDepth ? 1.0f : 0.0f;        
        
    return Shadow;
}

//------------------------------------------

void main()
{
	if ( TypeShader == GEOMETRY_RENDER ) // Geometry Render
	{
		out_Position = Position;
		out_Diffuse = texture( ColorMap, TexCoord ).xyz;
		out_Normal = normalize( Normal );
	}
	else if ( TypeShader == POINT_LIGHT ) // Point Light
	{
		vec2 texCoord = CalcTexCoord();
		vec3 position = texture( PositionMap, texCoord ).xyz;
		vec3 color = texture( ColorMap, texCoord ).rgb;
		vec3 Normal = texture( NormalMap, texCoord ).xyz;
		vec3 lightDirection = light.Position - position;
		float Distance = length( lightDirection );
		lightDirection = normalize( lightDirection );
		Normal = normalize( Normal );
						
		float DiffuseFactor = max( dot( lightDirection, Normal ), 0.0f );
		float Attenuation = 1 - pow( Distance / light.Radius, 2 );
		float Shadow = ShadowCalculation( position, Normal, lightDirection, light );
		
		vec4 DiffuseColor = ( light.Color * light.Intensivity * DiffuseFactor ) * Attenuation;
		
		out_Position = ( 1.0f - Shadow ) * DiffuseColor * vec4( color, 1.0f );
	}
}

//------------------------------------------