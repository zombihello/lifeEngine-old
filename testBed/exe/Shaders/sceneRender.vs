#version 330 core

#define GEOMETRY_RENDER 0
#define STENCIL_TEST 1
#define POINT_LIGHT 2

//------------------------------------------

layout ( location = 0 ) in vec3 in_Position;
layout ( location = 1 ) in vec3 in_Normal;
layout ( location = 2 ) in vec2 in_TexCoord;

//------------------------------------------

layout ( location = 0 ) out vec3 Position;
layout ( location = 1 ) out vec3 Normal;
layout ( location = 2 ) out vec2 TexCoord;

//------------------------------------------

uniform int TypeShader;

uniform mat4 PVTMatrix;
uniform mat4 TransformMatrix;

//------------------------------------------

void main()
{
	if ( TypeShader == GEOMETRY_RENDER ) // Geometry Render
	{
		Position = ( TransformMatrix * vec4( in_Position, 1.0f ) ).xyz;
		Normal = ( TransformMatrix * vec4( in_Normal, 0.0f ) ).xyz;
		
		gl_Position = PVTMatrix * vec4( in_Position, 1.0f );		
		TexCoord = in_TexCoord;
	}
	else // Stencil Test or Point Light
		gl_Position = PVTMatrix * vec4( in_Position, 1.0f );
}

//------------------------------------------