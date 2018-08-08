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

	glm::vec2 Size;
	Image Image;

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

		Image.loadFromFile( NameLightmaps[ Face ] );
		Size = glm::vec2( Image.getSize().x, Image.getSize().y );

		glGenTextures( 1, &Plane.LightMap );
		glBindTexture( GL_TEXTURE_2D, Plane.LightMap );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, Size.x, Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image.getPixelsPtr() );

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