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
uniform sampler2D ShadowMap;

//------------------------------------------

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / ScreenSize;
}

//------------------------------------------

vec2 GetShadowTC( vec3 Dir)
{
	float Sc;
	float Tc;
	float Ma;
	float FaceIndex;

	float rx = Dir.x;
	float ry = Dir.y;
	float rz = Dir.z;
	vec3 adir = abs( Dir );
	Ma = max( max( adir.x, adir.y ), adir.z );
	if ( adir.x > adir.y && adir.x > adir.z )
	{
		Sc = ( rx > 0.0 ) ? rz : -rz;
		Tc = ry;
		FaceIndex = ( rx > 0.0 ) ? 0.0 : 1.0;
	}
	else if ( adir.y > adir.x && adir.y > adir.z )
	{
		Sc = rx;
		Tc = ( ry > 0.0 ) ? rz : -rz;
		FaceIndex = ( ry > 0.0 ) ? 2.0 : 3.0;
	}
	else
	{
		Sc = ( rz > 0.0 ) ? -rx : rx;
		Tc = ry;
		FaceIndex = ( rz > 0.0 ) ? 4.0 : 5.0;
	}
	float s = 0.5 * ( Sc / Ma + 1.0 );
	float t = 0.5 * ( Tc / Ma + 1.0 );

	// кладём в атлас

	s = s / 3.0;
	t = t / 2.0;
	float Flr = floor(FaceIndex / 3.0);
	float Rmd = FaceIndex - (3.0 * Flr);
	s += Rmd / 3.0;
	t += Flr / 2.0;

	return vec2( s, t );
}

//------------------------------------------

float ShadowCalculation( vec4 PositionFragment )
{
    vec3 FragToLight = ( PositionFragment - light.Position ).xyz; 
	
	float CurrentDepth = length( FragToLight );
    float ClosestDepth = texture( ShadowMap, GetShadowTC( normalize(FragToLight) ) ).r * light.Radius;
	
    float Shadow = CurrentDepth > ClosestDepth ? 1.0f : 0.0f;        
        
    return Shadow;
}

//------------------------------------------

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 Normal = normalize( texture( NormalMap, texCoord ).xyz );	
	vec4 PositionFragment = texture( PositionMap, texCoord );
	
	vec3 lightDirection = ( light.Position - PositionFragment ).xyz;
	float Distance = length( lightDirection );
	lightDirection = normalize( lightDirection );
		
	float NdotL = dot( Normal, lightDirection );
	float DiffuseFactor = max( NdotL, 0.0f );
	float Attenuation =  max( 1.0f - pow( Distance / light.Radius, 2 ), 0.f );
	float Shadow = ShadowCalculation( PositionFragment );
		
	Color = ( 1.0f - Shadow ) * ( light.Color * DiffuseFactor * light.Intensivity ) * Attenuation * texture( ColorMap, texCoord ); 
}