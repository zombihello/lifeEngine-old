#version 330 core

//------------------------------------------

struct DirectionalLight
{
	mat4 LightMatrix;
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
uniform sampler2D ShadowMap;

//------------------------------------------

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / ScreenSize;
}

//------------------------------------------

float ShadowCalculation( vec4 PosFragInLightSpace, float NdotL )
{		
	vec3 ProjCoords = PosFragInLightSpace.xyz / PosFragInLightSpace.w;
	ProjCoords = ProjCoords * 0.5f + 0.5f;

	float Bias = max( 0.0015f * ( 1.0f - NdotL ), 0.00005f );
	float CurrentDepth = ProjCoords.z;
    float ClosestDepth = texture( ShadowMap, ProjCoords.xy ).r; 	

    float Shadow = CurrentDepth - Bias > ClosestDepth ? 1.0f : 0.0f;        
        
    return Shadow;
}

//------------------------------------------

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 Normal = normalize( texture( NormalMap, texCoord ).xyz );
	vec4 PositionFragment = texture( PositionMap, texCoord );
	vec3 ViewDirection = normalize( ViewPosition - PositionFragment.xyz );
	
	vec3 lightDirection = normalize( light.Position.xyz );
	float NdotL = dot( lightDirection, Normal );
	vec3 HalfwayDirection = normalize( lightDirection + ViewDirection );
	float DiffuseFactor = max( NdotL, 0.0f );	
	vec4 PosFragInLightSpace = light.LightMatrix * vec4(PositionFragment.xyz,1);
	float Shadow = ShadowCalculation( PosFragInLightSpace, NdotL );
	float SpecularFactor = pow( max( dot( Normal, HalfwayDirection ), 0.0 ), 32.0 );
	
	Color = ( 1.0f - Shadow ) * ( light.Color * DiffuseFactor * light.Intensivity + light.Specular * SpecularFactor ) * texture( ColorMap, texCoord );
}