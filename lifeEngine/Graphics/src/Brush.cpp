#include <System\VAO.h>
#include "..\Brush.h"

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
				glDeleteTextures( 1, &it->second[ i ].LightMap );
			}	
	}
}

//-------------------------------------------------------------------------//

void le::Brush::CreateBrush( const PrimitivesType& TypeBrush, const glm::vec3& Position, const GLuint& Texture, const vector<glm::vec3>& Vertex, const vector<glm::vec3>& Normals, const vector<glm::vec2>& TextureCoords )
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
		TempVertex.TextureCoord = TextureCoords[ i ];

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

	VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, BrushVertex, VAO::Static_Draw );
	
	for ( size_t Face = 0; Face < 6; Face++ )
	{
		BrushPlane Plane;

		Plane.ArrayBuffer = VAO::CreateVAO();
		VAO::BindVAO( Plane.ArrayBuffer );

		VAO::AtachBuffer( VAO::Vertex_Buffer, VertexBuffer );
		Plane.IndexBuffer = VAO::CreateBuffer( VAO::Index_Buffer, PlaneIdVertex[Face], VAO::Static_Draw );

		VAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( le::BrushVertex ), ( void* ) ( offsetof( le::BrushVertex, Position ) ) );
		VAO::SetVertexAttribPointer( VERT_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof( le::BrushVertex ), ( void* ) ( offsetof( le::BrushVertex, Normal ) ) );
		VAO::SetVertexAttribPointer( VERT_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof( le::BrushVertex ), ( void* ) ( offsetof( le::BrushVertex, TextureCoord ) ) );

		VAO::UnbindVAO();
		VAO::UnbindBuffer( VAO::Vertex_Buffer );
		VAO::UnbindBuffer( VAO::Index_Buffer );

		Plane.CountIndexs = PlaneIdVertex[ Face ].size();
		Plane.Brush = this;

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

bool le::BrushVertex::operator==( BrushVertex& BrushVertex )
{
	return Position == BrushVertex.Position && Normal == BrushVertex.Normal && TextureCoord == BrushVertex.TextureCoord;
}

//-------------------------------------------------------------------------//