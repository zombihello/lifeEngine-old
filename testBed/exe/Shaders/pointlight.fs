struct PointLight
{
	vec3 Position;
	vec4 Color;
	
	float Radius;
	float Intensivity;
};

uniform sampler2D PositionMap;
uniform sampler2D ColorMap;
uniform sampler2D NormalMap;
uniform vec2 ScreenSize;
uniform PointLight light;

out vec4 FragColor;

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / ScreenSize;
}

void main()
{		
	vec2 TexCoord = CalcTexCoord();
	vec3 WorlPos = texture( PositionMap, TexCoord ).xyz;
	vec3 Color = texture( ColorMap, TexCoord ).rgb;
	vec3 Normal = texture( NormalMap, TexCoord ).xyz;
	Normal = normalize( Normal );	

	vec3 lightDirection = light.Position - WorlPos;
	float Distance = length( lightDirection );
	lightDirection = normalize( lightDirection );
	
	float DiffuseFactor = max( 0.0, dot( Normal, lightDirection ) );
		
	float Attenuation = 1 - pow( Distance / light.Radius, 2 );
	vec4 DiffuseColor = ( light.Color * light.Intensivity * DiffuseFactor ) * Attenuation ;
	
	FragColor = vec4( Color, 1.0f ) * DiffuseColor;
}