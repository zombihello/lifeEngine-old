#version 330 core

//------------------------------------------

layout ( location = 0 ) in vec3 in_Position;

//------------------------------------------

out vec3 TexCoords;

//------------------------------------------

uniform mat4 PVMatrix;
uniform vec3 PositionCamera;

//------------------------------------------

void main()
{	
	vec4 Pos = PVMatrix * vec4( in_Position + PositionCamera, 1.0f );
	gl_Position = Pos.xyww;
	TexCoords = in_Position;
}