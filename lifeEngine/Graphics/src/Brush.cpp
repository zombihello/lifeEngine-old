#include <System\VAO.h>
#include "..\Brush.h"
#include <System\Logger.h>
#include <string>

//-------------------------------------------------------------------------//

le::Brush::Brush() :
	VertexBuffer( 0 )
{}

//-------------------------------------------------------------------------//

le::Brush::~Brush()
{
	if ( VertexBuffer != 0 )
	{
		VAO::DeleteBuffer( &VertexBuffer );

		for ( auto it = Planes.begin(); it != Planes.end(); it++ )
			for ( size_t i = 0; i < it->second.size(); i++ )
			{
				VAO::DeleteBuffer( &it->second[ i ].IndexBuffer );
				VAO::DeleteVAO( &it->second[ i ].ArrayBuffer );
			}
	}
}

//-------------------------------------------------------------------------//

#include <fstream>

void le::Brush::CreateBrush( const PrimitivesType& TypeBrush, const glm::vec3& Position, const GLuint& Texture, const vector<glm::vec3>& Vertex, const vector<glm::vec3>& Normals, const vector<glm::vec2>& TextureCoords, const vector<glm::vec2>& TextureCoords_LightMap, const vector<string>& NameLightmaps )
{
	vector<BrushVertex> BrushVertex;
	vector<unsigned int> IdVertex, TmpIdVertex;

	BoundingBox.InitBox( Vertex );
	this->Position = Position;

	switch ( TypeBrush )
	{
	case Cube:
		TmpIdVertex =
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

	le::BrushVertex TempVertex;
	map<int, vector<unsigned int> > PlaneIdVertex;

	for ( size_t i = 0, Face = 0, VertexInFace = 0; i < TmpIdVertex.size(); i++, VertexInFace++ )
	{
		TempVertex.Position = Vertex[ TmpIdVertex[ i ] ];
		TempVertex.Normal = Normals[ i ];
		TempVertex.TextureCoord_DiffuseMap = TextureCoords[ i ];
		TempVertex.TextureCoord_LightMap = TextureCoords_LightMap[ i ];

		bool IsFind = false;

		for ( size_t j = 0; j < BrushVertex.size(); j++ )
			if ( TempVertex == BrushVertex[ j ] )
			{
				IdVertex.push_back( j );
				PlaneIdVertex[ Face ].push_back( j );
				IsFind = true;
				break;
			}

		if ( !IsFind )
		{
			IdVertex.push_back( BrushVertex.size() );
			PlaneIdVertex[ Face ].push_back( BrushVertex.size() );
			BrushVertex.push_back( TempVertex );
		}

		if ( VertexInFace == 5 )
		{
			Face++;
			VertexInFace = -1;
		}
	}

	vector<Image> Lights;
	glm::vec2 Size;
	Image* Image;

	for ( size_t Face = 0; Face < 6; Face++ )
	{
		Lights.push_back( sf::Image() );
		Image = &Lights[ Lights.size()-1 ];
		Image->loadFromFile( NameLightmaps[Face] );
		Size = glm::vec2( Image->getSize().x, Image->getSize().y );

		for ( size_t i = 0, j = 0; i < PlaneIdVertex[ Face ].size() / 3; i++, j += 3 )
		{
			le::BrushVertex* A = &BrushVertex[ PlaneIdVertex[ Face ][ j ] ];
			le::BrushVertex* B = &BrushVertex[ PlaneIdVertex[ Face ][ j + 1 ] ];
			le::BrushVertex* C = &BrushVertex[ PlaneIdVertex[ Face ][ j + 2 ] ];

			glm::vec3 Normal = glm::normalize( glm::cross( B->Position - A->Position, C->Position - A->Position ) );

			if ( fabs( Normal.x ) > fabs( Normal.y ) && fabs( Normal.x ) > fabs( Normal.z ) )
			{
				A->TextureCoord_LightMap.x = A->Position.y;
				A->TextureCoord_LightMap.y = A->Position.z;

				B->TextureCoord_LightMap.x = B->Position.y;
				B->TextureCoord_LightMap.y = B->Position.z;

				C->TextureCoord_LightMap.x = C->Position.y;
				C->TextureCoord_LightMap.y = C->Position.z;
			}
			else if ( fabs( Normal.y ) > fabs( Normal.x ) && fabs( Normal.y ) > fabs( Normal.z ) )
			{
				A->TextureCoord_LightMap.x = A->Position.x;
				A->TextureCoord_LightMap.y = A->Position.z;

				B->TextureCoord_LightMap.x = B->Position.x;
				B->TextureCoord_LightMap.y = B->Position.z;

				C->TextureCoord_LightMap.x = C->Position.x;
				C->TextureCoord_LightMap.y = C->Position.z;
			}
			else
			{
				A->TextureCoord_LightMap.x = A->Position.x;
				A->TextureCoord_LightMap.y = A->Position.y;

				B->TextureCoord_LightMap.x = B->Position.x;
				B->TextureCoord_LightMap.y = B->Position.y;

				C->TextureCoord_LightMap.x = C->Position.x;
				C->TextureCoord_LightMap.y = C->Position.y;
			}

			glm::vec2 UVMin = A->TextureCoord_LightMap;
			glm::vec2 UVMax = A->TextureCoord_LightMap;

			for ( int n = 0; n < 3; n++ )
			{
				le::BrushVertex* Vertex = &BrushVertex[ PlaneIdVertex[ Face ][ n + j ] ];

				if ( Vertex->TextureCoord_LightMap.x < UVMin.x )
					UVMin.x = Vertex->TextureCoord_LightMap.x;

				if ( Vertex->TextureCoord_LightMap.y < UVMin.y )
					UVMin.y = Vertex->TextureCoord_LightMap.y;

				if ( Vertex->TextureCoord_LightMap.x > UVMax.x )
					UVMax.x = Vertex->TextureCoord_LightMap.x;

				if ( Vertex->TextureCoord_LightMap.y > UVMax.y )
					UVMax.y = Vertex->TextureCoord_LightMap.y;
			}

			glm::vec2 UVDelta;
			UVDelta.x = fabsf( UVMax.x - UVMin.x );
			UVDelta.y = fabsf( UVMax.y - UVMin.y );

			for ( int n = 0; n < 3; n++ )
			{
				le::BrushVertex* Vertex = &BrushVertex[ PlaneIdVertex[ Face ][ n + j ] ];
				Vertex->TextureCoord_LightMap.x = fabsf(( Vertex->TextureCoord_LightMap.x - UVMin.x ) / UVDelta.x);
				Vertex->TextureCoord_LightMap.y = fabsf( ( Vertex->TextureCoord_LightMap.y - UVMin.y ) / UVDelta.y );
			}
		}
	}

	VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, BrushVertex, VAO::Static_Draw );

	for ( size_t Face = 0, IdLightMap = 0; Face < 6; Face++, IdLightMap += 2 )
	{
		BrushPlane Plane;

		Plane.ArrayBuffer = VAO::CreateVAO();
		VAO::BindVAO( Plane.ArrayBuffer );

		VAO::AtachBuffer( VAO::Vertex_Buffer, VertexBuffer );
		Plane.IndexBuffer = VAO::CreateBuffer( VAO::Index_Buffer, PlaneIdVertex[ Face ], VAO::Static_Draw );

		VAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( le::BrushVertex ), ( void* ) ( offsetof( le::BrushVertex, Position ) ) );
		VAO::SetVertexAttribPointer( VERT_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof( le::BrushVertex ), ( void* ) ( offsetof( le::BrushVertex, Normal ) ) );
		VAO::SetVertexAttribPointer( VERT_TEXCOORD_DIFFUSE, 2, GL_FLOAT, GL_FALSE, sizeof( le::BrushVertex ), ( void* ) ( offsetof( le::BrushVertex, TextureCoord_DiffuseMap ) ) );
		VAO::SetVertexAttribPointer( VERT_TEXCOORD_LIGHTMAP, 2, GL_FLOAT, GL_FALSE, sizeof( le::BrushVertex ), ( void* ) ( offsetof( le::BrushVertex, TextureCoord_LightMap ) ) );

		VAO::UnbindVAO();
		VAO::UnbindBuffer( VAO::Vertex_Buffer );
		VAO::UnbindBuffer( VAO::Index_Buffer );

		Plane.CountIndexs = PlaneIdVertex[ Face ].size();
		Plane.Brush = this;

		for ( size_t i = 0; i < PlaneIdVertex[ Face ].size(); i++ )
			Plane.Vertexes.push_back( BrushVertex[ PlaneIdVertex[ Face ][ i ] ] );

		Image = &Lights[ Face ];
		Size = glm::vec2( Image->getSize().x, Image->getSize().y );

		glGenTextures( 1, &Plane.LightMap );
		glBindTexture( GL_TEXTURE_2D, Plane.LightMap );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, Size.x, Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image->getPixelsPtr() );

		Planes[ Texture ].push_back( Plane );
	}
}

//-------------------------------------------------------------------------//

bool le::Brush::IsVisible( Frustum& Frustum )
{
	return Frustum.IsVisible( BoundingBox );
}

//-------------------------------------------------------------------------//

bool le::Brush::IsVisible( Camera& Camera )
{
	return Camera.GetFrustum().IsVisible( BoundingBox );
}

//-------------------------------------------------------------------------//

bool le::Brush::IsVisible()
{
	return BoundingBox.Query.GetResult() > 0;
}

//-------------------------------------------------------------------------//

void le::Brush::QueryTest()
{
	BoundingBox.QueryTest();
}

//-------------------------------------------------------------------------//

void le::Brush::SetDistanceToCamera( Camera& Camera )
{
	DistanceToCamera = Camera.GetDistance( Position );
}

//-------------------------------------------------------------------------//

map<GLuint, vector<le::BrushPlane> >& le::Brush::GetPlanes()
{
	return Planes;
}

//-------------------------------------------------------------------------//

float le::Brush::GetDistanceToCamera()
{
	return DistanceToCamera;
}

//-------------------------------------------------------------------------//

glm::vec3& le::Brush::GetPosition()
{
	return Position;
}

//-------------------------------------------------------------------------//

le::BoundingBox& le::Brush::GetBoundingBox()
{
	return BoundingBox;
}

//-------------------------------------------------------------------------//