#version 330 core
#define MAX_BONES 255

//------------------------------------------

layout ( location = 0 ) in vec3 in_Position;
layout ( location = 2 ) in vec2 in_TexCoord;
layout ( location = 3 ) in vec4 in_IdBones;
layout ( location = 4 ) in vec4 in_Weights;

//------------------------------------------

out vec2 TexCoord;

//------------------------------------------

uniform mat4 PVTMatrix;
uniform mat4 [MAX_BONES] Bones;

//------------------------------------------

void main()
{
	mat4 BoneTrans = Bones[int(in_IdBones.x)] * in_Weights.x;
	BoneTrans += Bones[int(in_IdBones.y)] * in_Weights.y;
	BoneTrans += Bones[int(in_IdBones.z)] * in_Weights.z;
	BoneTrans += Bones[int(in_IdBones.w)] * in_Weights.w;
	
	TexCoord = in_TexCoord;
	gl_Position = PVTMatrix * ( vec4( in_Position, 1.0f ) * BoneTrans );	
}