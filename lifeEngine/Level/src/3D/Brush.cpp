#include "..\..\3D\Brush.h"
#include "..\..\..\System\LoaderVAO.h"

//-------------------------------------------------------------------------//

bool le::BrushVertex::operator==( BrushVertex& BrushVertex )
{
	return Vertex == BrushVertex.Vertex && Normal == BrushVertex.Normal && TextureCoord == BrushVertex.TextureCoord;
}

//-------------------------------------------------------------------------//

le::Brush::Brush( le::System& System, le::Scene3D& Scene, le::Physic3D& Physic )
{
	this->System = &System;
	this->Physic = &Physic;
	this->Scene = &Scene;
	Body = NULL;

	VertexBuffer = VertexArray = IndexBuffer = 0;
}

//-------------------------------------------------------------------------//

le::Brush::Brush( le::System& System, le::Physic3D& Physic )
{
	this->System = &System;
	this->Physic = &Physic;
	Scene = NULL;
	Body = NULL;

	VertexBuffer = VertexArray = IndexBuffer = 0;
}

//-------------------------------------------------------------------------//

le::Brush::~Brush()
{
	if ( Body != NULL )
		delete Body;

	if ( VertexBuffer != 0 )
		LoaderVAO::DeleteBuffer( &VertexBuffer );

	if ( IndexBuffer != 0 )
		LoaderVAO::DeleteBuffer( &IndexBuffer );

	if ( VertexArray != 0 )
		LoaderVAO::DeleteVAO( &VertexArray );
}

//-------------------------------------------------------------------------//

void le::Brush::CreateBrush( PrimitivesType Type, GLuint Texture, vector<glm::vec3> Vertex, vector<glm::vec3> Normals, vector<glm::vec2> TextureCoords )
{
	vector<le::BrushVertex> vBrushVertex;
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
		vCollision_Vertex.push_back( Vertex[i].x );
		vCollision_Vertex.push_back( Vertex[i].y );
		vCollision_Vertex.push_back( Vertex[i].z );
	}

	for ( int i = 0; i < tmpIdVertex.size(); i++ )
	{
		BrushVertex tmpVertex;
		tmpVertex.Vertex = Vertex[tmpIdVertex[i]];
		tmpVertex.Normal = Normals[i];
		tmpVertex.TextureCoord = TextureCoords[i];

		bool isFind = false;
		for ( int j = 0; j < vBrushVertex.size(); j++ )
		if ( tmpVertex == vBrushVertex[j] )
		{
			vIdVertex[i] = j;
			isFind = true;
			break;
		}

		if ( !isFind )
		{
			for ( int j = i; j < tmpIdVertex.size(); j++ )
			if ( tmpIdVertex[j] == tmpIdVertex[i] )
				vIdVertex[j] = vBrushVertex.size();

			vBrushVertex.push_back( tmpVertex );
		}
	}

	Body3D_ConstructionInfo constructionInfo( Body3D_ConstructionInfo::Static, 0, Vector3f(), Vector3f() );
	Body = new Body3D( *Physic, &constructionInfo, ShapeType_Mesh( &vCollision_Vertex, &vCollision_IdVertex ) );

	VertexArray = LoaderVAO::CreateVAO();
	LoaderVAO::BindVAO( VertexArray );

	VertexBuffer = LoaderVAO::AtachBuffer( GL_ARRAY_BUFFER, vBrushVertex, GL_STATIC_DRAW );
	IndexBuffer = LoaderVAO::AtachBuffer( GL_ELEMENT_ARRAY_BUFFER, vIdVertex, GL_STATIC_DRAW );

	LoaderVAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( BrushVertex ), ( void* ) ( offsetof( BrushVertex, BrushVertex::Vertex ) ) );
	LoaderVAO::SetVertexAttribPointer( VERT_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof( BrushVertex ), ( void* ) ( offsetof( BrushVertex, BrushVertex::Normal ) ) );
	LoaderVAO::SetVertexAttribPointer( VERT_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof( BrushVertex ), ( void* ) ( offsetof( BrushVertex, BrushVertex::TextureCoord ) ) );

	LoaderVAO::UnbindVAO();
	LoaderVAO::UnbindBuffer( GL_ARRAY_BUFFER );
	LoaderVAO::UnbindBuffer( GL_ELEMENT_ARRAY_BUFFER );

	MatrixTransformation = glm::translate( glm::vec3( 0, 0, 0 ) );

	Material MaterialBrush;
	MaterialBrush.Texture = Texture;

	SceneInfoMesh InfoMesh;
	InfoMesh.CountIndexs = vIdVertex.size();
	InfoMesh.VertexArray = VertexArray;
	InfoMesh.MatrixTransformation = &MatrixTransformation;
	InfoMesh.Material = MaterialBrush;

	mRenderMesh[Texture] = InfoMesh;
}

//-------------------------------------------------------------------------//

void le::Brush::UpdateBrush( Shader* Shader )
{
	if ( Scene != NULL )
		Scene->AddMeshToScene( mRenderMesh );
}

//-------------------------------------------------------------------------//

void le::Brush::AddToScene( le::Scene3D& Scene )
{
	this->Scene = &Scene;
}

//-------------------------------------------------------------------------//