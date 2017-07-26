#version 330 core

struct PointLight
{
	vec3 Position;
	vec4 Color;
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
	vec3 lightDir = normalize( light.Position - WorlPos );
	
	float nDotl = dot( Normal, lightDir );
	float brightness = max( nDotl, 0.0f );
	vec3 diffuse = brightness * light.Color;
	
	FragColor = vec4( Color, 1.0f ) * vec4( diffuse, 1.0f ) ;
	}