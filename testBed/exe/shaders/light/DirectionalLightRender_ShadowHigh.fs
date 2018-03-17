#version 330 core

//------------------------------------------

struct DirectionalLight
{
	mat4 LightMatrix;
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
uniform sampler2D ShadowMap;

//------------------------------------------

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / ScreenSize;
}

//------------------------------------------

float ShadowCalculation( vec2 TexCoords, float CurrentDepth )
{		
    float ClosestDepth = texture( ShadowMap, TexCoords.xy ).r; 	
    float Shadow = CurrentDepth > ClosestDepth ? 1.0f : 0.0f;        
        
    return Shadow;
}

//------------------------------------------

float ShadowCalculationLiner( vec2 TexCoords, vec2 TexelSize, float CurrentDepth )
{
	vec2 PixelPos = TexCoords / TexelSize + 0.5f;
	vec2 FractPart = fract( PixelPos );
	vec2 StartTexel = ( PixelPos - FractPart ) * TexelSize;
	
	float blTexel = ShadowCalculation( StartTexel, CurrentDepth );
	float brTexel = ShadowCalculation( StartTexel + vec2( TexelSize.x, 0.0f ), CurrentDepth );
	float tlTexel = ShadowCalculation( StartTexel + vec2( 0.0f, TexelSize.y ), CurrentDepth );
	float trTexel = ShadowCalculation( StartTexel + TexelSize, CurrentDepth );
	
	float MixA = mix( blTexel, tlTexel, FractPart.y );
	float MixB = mix( brTexel, trTexel, FractPart.y );
	
	return mix( MixA, MixB, FractPart.x );
}

//------------------------------------------

float ShadowCalculationPCF( vec4 PosFragInLightSpace )
{
    vec3 ProjCoords = PosFragInLightSpace.xyz / PosFragInLightSpace.w;
	ProjCoords = ProjCoords * 0.5f + 0.5f;
	vec2 TexelSize = 1.0f / textureSize( ShadowMap, 0 );
	
	float CurrentDepth = ProjCoords.z;
	float Shadow = 0.0f;
	
	for ( int x = -1; x <= 1; ++x )
		for ( int y = -1; y <= 1; ++y )
			Shadow += ShadowCalculationLiner( ProjCoords.xy + vec2( x, y ) * TexelSize, TexelSize, CurrentDepth );
	           
    return Shadow / 9.0f;
}

//------------------------------------------

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 Normal = normalize( texture( NormalMap, texCoord ).xyz );
	vec4 PositionFragment = texture( PositionMap, texCoord );

	float DiffuseFactor = max( dot( normalize( light.Position.xyz ), Normal ), 0.0f );	
	vec4 PosFragInLightSpace = light.LightMatrix * vec4(PositionFragment.xyz,1);
	float Shadow = ShadowCalculationPCF( PosFragInLightSpace );
	
	Color =  ( 1.0f - Shadow ) * light.Color * DiffuseFactor * light.Intensivity * texture( ColorMap, texCoord );
}