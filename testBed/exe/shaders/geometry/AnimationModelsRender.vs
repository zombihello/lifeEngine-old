#version 330 core
#define MAX_BONES 255

//------------------------------------------

layout ( location = 0 ) in vec3 in_Position;
layout ( location = 1 ) in vec3 in_Normal;
layout ( location = 2 ) in vec2 in_TexCoord;
layout ( location = 3 ) in vec4 in_IdBones;
layout ( location = 4 ) in vec4 in_Weights;

//------------------------------------------

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

//------------------------------------------

uniform mat4 PVMatrix;
uniform mat4 TransformMatrix;
uniform mat4[MAX_BONES] Bones;

//------------------------------------------

void main()
{
	mat4 BoneTrans = Bones[int(in_IdBones.x)] * in_Weights.x;
	BoneTrans += Bones[int(in_IdBones.y)] * in_Weights.y;
	BoneTrans += Bones[int(in_IdBones.z)] * in_Weights.z;
	BoneTrans += Bones[int(in_IdBones.w)] * in_Weights.w;
	
	Position = ( TransformMatrix * ( vec4( in_Position, 1.0f ) * BoneTrans ) ).xyz;
	Normal =  ( TransformMatrix * (  vec4( in_Normal, 0.0f ) * BoneTrans ) ).xyz;
	TexCoord = in_TexCoord;
	
	gl_Position = PVMatrix * vec4( Position, 1.0f );	
}