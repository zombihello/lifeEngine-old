layout (location = 0) out vec3 worldPos;
layout (location = 1) out vec3 Diffuse;
layout (location = 2) out vec3 normal;

uniform sampler2D gColorMap;

in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos;

void main()
{	
	worldPos = WorldPos;
	Diffuse = texture( gColorMap, TexCoord );
	normal = normalize( Normal );
}