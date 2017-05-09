#include "..\..\3D\Brush.h"

//-------------------------------------------------------------------------//

le::Brush::Brush( le::System& System )
{
	this->System = &System;
	TextureBrush = 0;
}

//-------------------------------------------------------------------------//

le::Brush::~Brush()
{
	if ( TextureBrush != 0 )
		glDeleteTextures( 1, &TextureBrush );

	vVertex.clear();
	vTextureCoords.clear();
	vIdVertex.clear();
}

//-------------------------------------------------------------------------//

void le::Brush::InitBrush( PrimitivesType Type, GLuint Texture, vector<Vector3f> Vertex, vector<Vector2f> TextureCoords )
{
	switch ( Type )
	{
	case Cube:
		vIdVertex =
		{
			7, 3, 4,
			3, 0, 4,

			2, 6, 1,
			6, 5, 1,

			7, 6, 3,
			6, 2, 3,

			0, 1, 4,
			1, 5, 4,

			6, 4, 5,
			6, 7, 4,

			0, 2, 1,
			0, 3, 2
		};
		break;

	case Sphere:
		break;

	case Plane:
		break;
	}

	TextureBrush = Texture;
	vVertex = Vertex;
	vTextureCoords = TextureCoords;
}

//-------------------------------------------------------------------------//

void le::Brush::RenderBrush()
{
	glBindTexture( GL_TEXTURE_2D, TextureBrush );
	glBegin( GL_TRIANGLES );

	for ( int i = 0; i < vIdVertex.size(); i++ )
	{
		int id = vIdVertex[i];

		Vector3f Vertex = vVertex[id];
		Vector2f TexCoord = vTextureCoords[i];

		glTexCoord2f( TexCoord.x, TexCoord.y );
		glVertex3f( Vertex.x, Vertex.y, Vertex.z );
	}

	glEnd();
}

//-------------------------------------------------------------------------//