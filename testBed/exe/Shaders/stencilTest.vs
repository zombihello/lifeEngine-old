layout (location = 0) in vec3 position;

uniform mat4 PVTMatrix;

void main()
{
	gl_Position = PVTMatrix * vec4( position, 1.0f );
}