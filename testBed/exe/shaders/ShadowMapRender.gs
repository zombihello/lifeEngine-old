#version 330 core

//------------------------------------------

layout ( triangles ) in;
layout ( triangle_strip, max_vertices = 18 ) out;

//------------------------------------------

uniform int CountFace;
uniform mat4 ShadowMatrices[ 6 ];

//------------------------------------------

out vec4 FragPos;

//------------------------------------------

void main()
{
	for ( int Face = 0; Face < CountFace; Face++ )
	{
		gl_Layer = Face;
		
		for ( int i = 0; i < 3; i++ )
		{
			FragPos = gl_in[ i ].gl_Position;
			gl_Position = ShadowMatrices[ Face ] * FragPos;
			EmitVertex();
		}
		
		EndPrimitive();
	}
}

//------------------------------------------