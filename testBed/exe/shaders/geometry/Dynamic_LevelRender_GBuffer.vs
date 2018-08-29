#version 330 core

//------------------------------------------

layout ( location = 0 ) in vec3 in_Position;
layout ( location = 1 ) in vec3 in_Normal;
layout ( location = 2 ) in vec2 in_TexCoord;
layout ( location = 5 ) in vec2 in_TexCoord_LightMap;

//------------------------------------------

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;
out vec2 TexCoord_LightMap;

//------------------------------------------

uniform mat4 PVMatrix;
uniform mat4 TransformationMatrix;

//------------------------------------------

void main()
{		
	Position = ( TransformationMatrix * vec4( in_Position, 1.0f ) ).xyz;	
	Normal = ( TransformationMatrix * vec4( in_Normal, 0.0f ) ).xyz;	
	TexCoord = in_TexCoord;
	TexCoord_LightMap = in_TexCoord_LightMap;
	
	gl_Position = PVMatrix * vec4( Position, 1.0f );
}