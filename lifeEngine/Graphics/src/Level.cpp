#include <System\Logger.h>
#include <System\ResourcesManager.h>
#include <Graphics\Camera.h>
#include <Graphics\Scene.h>
#include <Graphics\LightManager.h>
#include "..\Level.h"

//-------------------------------------------------------------------------//

le::Level::Level( System& System ) :
	Scene( NULL ),
	VertexBuffer( 0 ),
	IndexBuffer( 0 ),
	ArrayBuffer( 0 )
{
	Skybox = new le::Skybox();
	Skybox->SetSizeSkybox( 500 );
}

//-------------------------------------------------------------------------//

le::Level::~Level()
{
	if ( !Planes.empty() )
	{
		for ( auto It = Planes.begin(); It != Planes.end(); It++ )
			for ( size_t i = 0; i < It->second.size(); i++ )
				delete Planes[ It->first ][ i ];

		Planes.clear();
	}

	if ( VertexBuffer != 0 )
	{
		VAO::DeleteVAO( &ArrayBuffer );
		VAO::DeleteBuffer( &VertexBuffer );
		VAO::DeleteBuffer( &IndexBuffer );
	}

	delete Skybox;
}

//-------------------------------------------------------------------------//

bool le::Level::LoadLevel( const string& Route )
{
	BSPLoad BSPLoad;

	if ( !BSPLoad.LoadBSP( Route ) )
		return false;

	// *****************************************
	// Получаем массив вершин, индексов и фейсов

	int NumberVertexes = BSPLoad.GetNumberVertexs();
	BSPVertex* Vertexes = BSPLoad.GetPtrVertexs();

	int NumberIndices = BSPLoad.GetNumberIndices();
	unsigned int* Indeces = BSPLoad.GetPtrIndeces();

	int NumberFaces = BSPLoad.GetNumberFaces();
	BSPFace* Faces = BSPLoad.GetPtrFaces();

	GLuint* Textures = BSPLoad.GetPtrTextures();

	// *****************************************
	// Инициализируем плоскости

	for ( int i = 0; i < NumberFaces; i++ )
	{
		if ( Faces[ i ].Type != 1 ) continue;

		Plane* Plane = new le::Plane();
		Plane->InitPlane( Faces[ i ] );
		Planes[ Textures[ Faces[ i ].TextureID ] ].push_back( Plane );
	}

	// *****************************************
	// Создаем VAO

	ArrayBuffer = VAO::CreateVAO();
	VAO::BindVAO( ArrayBuffer );

	VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, NumberVertexes, &Vertexes[ 0 ], VAO::Static_Draw );
	IndexBuffer = VAO::CreateBuffer( VAO::Index_Buffer, NumberIndices, &Indeces[ 0 ], VAO::Static_Draw );

	VAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( le::BSPVertex ), ( void* ) ( offsetof( le::BSPVertex, Position ) ) );
	VAO::SetVertexAttribPointer( VERT_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof( le::BSPVertex ), ( void* ) ( offsetof( le::BSPVertex, Normal ) ) );
	VAO::SetVertexAttribPointer( VERT_TEXCOORD_DIFFUSE, 2, GL_FLOAT, GL_FALSE, sizeof( le::BSPVertex ), ( void* ) ( offsetof( le::BSPVertex, TextureCoord ) ) );
	VAO::SetVertexAttribPointer( VERT_TEXCOORD_LIGHTMAP, 2, GL_FLOAT, GL_FALSE, sizeof( le::BSPVertex ), ( void* ) ( offsetof( le::BSPVertex, LightmapCoord ) ) );

	VAO::UnbindVAO();
	VAO::UnbindBuffer( VAO::Vertex_Buffer );
	VAO::UnbindBuffer( VAO::Index_Buffer );

	return true;
}

//-------------------------------------------------------------------------//

void le::Level::AddToScene( le::Scene& Scene )
{
	this->Scene = &Scene;
	Scene.AddLevelToScene( this );
}

//-------------------------------------------------------------------------//

void le::Level::RemoveFromScene()
{
	if ( Scene )
		Scene->RemoveLevelFromScene( this );
}

//-------------------------------------------------------------------------//

void le::Level::SetScene( le::Scene* Scene )
{
	this->Scene = Scene;
}

//-------------------------------------------------------------------------//

void le::Level::ClearLevel()
{
	if ( !Planes.empty() )
	{
		for ( auto It = Planes.begin(); It != Planes.end(); It++ )
			for ( size_t i = 0; i < It->second.size(); i++ )
				delete Planes[ It->first ][ i ];

		Planes.clear();
	}

	if ( VertexBuffer != 0 )
	{
		VAO::DeleteVAO( &ArrayBuffer );
		VAO::DeleteBuffer( &VertexBuffer );
		VAO::DeleteBuffer( &IndexBuffer );
	}
}

//-------------------------------------------------------------------------//

le::Skybox& le::Level::GetSkybox()
{
	return *Skybox;
}

//-------------------------------------------------------------------------//

GLuint& le::Level::GetArrayBuffer()
{
	return ArrayBuffer;
}

//-------------------------------------------------------------------------//

le::Entity* le::Level::GetEntity( const string& NameEntity )
{
	for ( size_t i = 0; i < Entitys.size(); i++ )
		if ( Entitys[ i ].GetNameEntity() == NameEntity )
			return &Entitys[ i ];

	return NULL;
}

//-------------------------------------------------------------------------//

map<GLuint, vector<le::Plane*> >& le::Level::GetAllPlanes()
{
	return Planes;
}

//-------------------------------------------------------------------------//

vector<le::Entity>& le::Level::GetAllEntitys()
{
	return Entitys;
}

//-------------------------------------------------------------------------//

