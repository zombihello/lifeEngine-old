layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 texcoord;

uniform mat4 PVTMatrix;
uniform mat4 TransformMatrix;

out vec2 TexCoord;
out vec3 Normal;
out vec3 WorldPos;

void main()
{
	gl_Position = PVTMatrix * vec4( position, 1.0f );
	TexCoord = texcoord;
	Normal = ( TransformMatrix * vec4( normal, 0.0f ) ).xyz;
	WorldPos = ( TransformMatrix * vec4( position, 1.0f ) ).xyz;
}