layout (location = 0) in vec3 position;
layout (location = 2) in vec2 textcoord;

uniform mat4 PVTMatrix;

void main()
{
	gl_Position = PVTMatrix * vec4( position, 1.0f );
}