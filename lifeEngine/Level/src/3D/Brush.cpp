#include "..\..\3D\Brush.h"

//-------------------------------------------------------------------------//

bool le::BrushVertex::operator==( BrushVertex& BrushVertex )
{
	return Vertex == BrushVertex.Vertex && TextureCoord == BrushVertex.TextureCoord;
}

//-------------------------------------------------------------------------//

le::Brush::Brush( le::System& System, le::Physic3D& Physic )
{
	this->System = &System;
	this->Physic = &Physic;
	Body = NULL;
	TextureBrush = VertexBuffer = IndexBuffer = 0;
	iCountIndex = 0;
}

//-------------------------------------------------------------------------//

le::Brush::~Brush()
{
	if ( Body != NULL )
		delete Body;

	if ( VertexBuffer != 0 )
		glDeleteBuffers( 1, &VertexBuffer );

	if ( IndexBuffer != 0 )
		glDeleteBuffers( 1, &IndexBuffer );
}

//-------------------------------------------------------------------------//

void le::Brush::CreateBrush( PrimitivesType Type, GLuint Texture, vector<Vector3f> Vertex, vector<Vector2f> TextureCoords )
{
	vector<BrushVertex> vBrushVertex;
	vector<unsigned int> vIdVertex;
	vector<unsigned int> tmpIdVertex;

	switch ( Type )
	{
	case Cube:
		vIdVertex = tmpIdVertex =
		{
			7, 3, 4, 3, 0, 4, 2, 6, 1,
			6, 5, 1, 7, 6, 3, 6, 2, 3,
			0, 1, 4, 1, 5, 4, 6, 4, 5,
			6, 7, 4, 0, 2, 1, 0, 3, 2
		};

		vCollision_IdVertex = 
		{
			7, 3, 4, 3, 0, 4, 2, 6, 1,
			6, 5, 1, 7, 6, 3, 6, 2, 3,
			0, 1, 4, 1, 5, 4, 6, 4, 5,
			6, 7, 4, 0, 2, 1, 0, 3, 2
		};

		break;

	case Sphere:
		break;

	case Plane:
		break;
	}

	for ( int i = 0; i < Vertex.size(); i++ )
	{
		vCollision_Vertex.push_back( Vertex[ i ].x );
		vCollision_Vertex.push_back( Vertex[ i ].y );
		vCollision_Vertex.push_back( Vertex[ i ].z );
	}

	for ( int i = 0; i < tmpIdVertex.size(); i++ )
	{
		BrushVertex tmpVertex;
		tmpVertex.Vertex = Vertex[ tmpIdVertex[ i ] ];
		tmpVertex.TextureCoord = TextureCoords[ i ];

		bool isFind = false;
		for ( int j = 0; j < vBrushVertex.size(); j++ )
		if ( tmpVertex == vBrushVertex[ j ] )
		{
			vIdVertex[ i ] = j;
			isFind = true;
			break;
		}

		if ( !isFind )
		{
			for ( int j = i; j < tmpIdVertex.size(); j++ )
			if ( tmpIdVertex[ j ] == tmpIdVertex[ i ] )
				vIdVertex[ j ] = vBrushVertex.size();

			vBrushVertex.push_back( tmpVertex );
		}
	}

	TextureBrush = Texture;
	iCountIndex = vIdVertex.size();

	Body3D_ConstructionInfo constructionInfo( Body3D_ConstructionInfo::Static, 0, Vector3f(), Vector3f() );
	Body = new Body3D( *Physic, &constructionInfo, ShapeType_Mesh( &vCollision_Vertex, &vCollision_IdVertex ) );

	glGenBuffers( 1, &VertexBuffer );
	glGenBuffers( 1, &IndexBuffer );

	glBindBuffer( GL_ARRAY_BUFFER, VertexBuffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBuffer );

	glBufferData( GL_ARRAY_BUFFER, vBrushVertex.size() * sizeof( BrushVertex ), vBrushVertex.data(), GL_STATIC_DRAW );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, vIdVertex.size() * sizeof( unsigned int ), vIdVertex.data(), GL_STATIC_DRAW );

}

//-------------------------------------------------------------------------//

void le::Brush::RenderBrush()
{
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glBindTexture( GL_TEXTURE_2D, TextureBrush );
	glBindBuffer( GL_ARRAY_BUFFER, VertexBuffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBuffer );

	glVertexPointer( 3, GL_FLOAT, sizeof( BrushVertex ), ( void* ) ( offsetof( BrushVertex, Vertex ) ) );
	glTexCoordPointer( 2, GL_FLOAT, sizeof( BrushVertex ), ( void* ) ( offsetof( BrushVertex, TextureCoord ) ) );

	glDrawElements( GL_TRIANGLES, iCountIndex, GL_UNSIGNED_INT, 0 );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}

//-------------------------------------------------------------------------//