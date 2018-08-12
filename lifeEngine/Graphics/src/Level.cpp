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
	if ( !ArrayPlanes.empty() )
	{
		for ( auto It = ArrayPlanes.begin(); It != ArrayPlanes.end(); It++ )
			for ( size_t i = 0; i < It->second.size(); i++ )
				delete ArrayPlanes[ It->first ][ i ];

		ArrayPlanes.clear();
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
	// *************************************
	// Открываем файл карты

	ifstream File( Route, ios::binary );

	if ( !File.is_open() )
	{
		Logger::Log( Logger::Error, "BSP File [" + Route + "] Not Found" );
		return false;
	}

	int				NumberVertexes = 0;
	int				NumberFaces = 0;
	int				NumberIndices = 0;
	int				NumberTextures = 0;
	unsigned int*	Array_Indices = NULL;

	BSPHeader		Header = { 0 };
	BSPLump			Lumps[ MaxLumps ] = { 0 };
	BSPVertex*		Array_Vertexes = NULL;
	BSPFace*		Array_Faces = NULL;
	BSPTexture*		Array_Textures = NULL;
	vector<GLuint>	GLTextures;

	// *************************************
	// Загружаем заголовок и куски файла

	File.read( ( char* ) &Header, sizeof( BSPHeader ) );
	File.read( ( char* ) &Lumps, MaxLumps * sizeof( BSPLump ) );

	if ( string( Header.StrID, 4 ) != "LBSP" || Header.Version != 10 )
	{
		Logger::Log( Logger::Error, "Not Supported Format BSP [ Type: " + string( Header.StrID, 4 ) + " Version: " + to_string( Header.Version ) + "]" );
		return false;
	}

	// Выделяем память для вершин
	NumberVertexes = Lumps[ Vertices ].Length / sizeof( BSPVertex );
	Array_Vertexes = new BSPVertex[ NumberVertexes ];

	// Выделяем память для фейсов
	NumberFaces = Lumps[ Faces ].Length / sizeof( BSPFace );
	Array_Faces = new BSPFace[ NumberFaces ];

	// Выделяем память для индексов
	NumberIndices = Lumps[ Indices ].Length / sizeof( unsigned int );
	Array_Indices = new unsigned int[ NumberIndices ];

	// Выделяем память для текстурной информации
	NumberTextures = Lumps[ Textures ].Length / sizeof( BSPTexture );
	Array_Textures = new BSPTexture[ NumberTextures ];

	// Смещаемся на участок в файле, в котором хранится информация о вершинах
	File.seekg( Lumps[ Vertices ].Offset, ios::beg );

	// Считываем все вершины с файла
	for ( int i = 0; i < NumberVertexes; i++ )
	{
		// Считываем текущую вершину
		File.read( ( char* ) &Array_Vertexes[ i ], sizeof( BSPVertex ) );

		// Меняем значения Y и Z, и отрицаем новый Z, чтобы Y был вверх.
		float Temp = Array_Vertexes[ i ].Position.y;
		Array_Vertexes[ i ].Position.y = Array_Vertexes[ i ].Position.z;
		Array_Vertexes[ i ].Position.z = -Temp;
	
		Temp = Array_Vertexes[ i ].Normal.y;
		Array_Vertexes[ i ].Normal.y = Array_Vertexes[ i ].Normal.z;
		Array_Vertexes[ i ].Normal.z = -Temp;

		Array_Vertexes[ i ].TextureCoord.y = -Array_Vertexes[ i ].TextureCoord.y;
	}

	// Смещаемся на участок в файле, в котором хранится информация о индексах
	File.seekg( Lumps[ Indices ].Offset, ios::beg );
	File.read( ( char* ) &Array_Indices[ 0 ], NumberIndices * sizeof( unsigned int ) );

	// Смещаемся на участок в файле, в котором хранится информация о фейсах
	File.seekg( Lumps[ Faces ].Offset, ios::beg );
	File.read( ( char* ) &Array_Faces[ 0 ], NumberFaces * sizeof( BSPFace ) );

	// Смещаемся на участок в файле, в котором хранится информация о текстурах
	File.seekg( Lumps[ Textures ].Offset, ios::beg );
	File.read( ( char* ) &Array_Textures[ 0 ], NumberTextures * sizeof( BSPTexture ) );

	// Загружаем все текстуры
	string RouteToTexture;

	for ( int i = 0; i < NumberTextures; i++ )
	{
		// Определяем формат текстуры
		RouteToTexture = "../" + string( Array_Textures[ i ].StrName );

		File.open( RouteToTexture + ".jpg", ios::binary );

		if ( File.is_open() )
			RouteToTexture += ".jpg";
		else
		{
			File.open( RouteToTexture + ".tga", ios::binary );

			if ( File.is_open() )
				RouteToTexture += ".tga";
		}

		File.clear();
		File.close();

		// Загружаем текстуру
		ResourcesManager::LoadGlTexture( Array_Textures[ i ].StrName, RouteToTexture );
		GLTextures.push_back( ResourcesManager::GetGlTexture( Array_Textures[ i ].StrName ) );
	}

	// *****************************************
	// Инициализируем плоскости

	for ( int i = 0; i < NumberFaces; i++ )
	{
		if ( Array_Faces[ i ].Type != 1 ) continue;

		Plane*		Plane = new le::Plane();
		BSPFace*	Face = &Array_Faces[ i ];

		Plane->InitPlane( *Face );
		ArrayPlanes[ GLTextures[ Face->TextureID ] ].push_back( Plane );
	}

	// *****************************************
	// Создаем VAO

	ArrayBuffer = VAO::CreateVAO();
	VAO::BindVAO( ArrayBuffer );

	VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, NumberVertexes, &Array_Vertexes[ 0 ], VAO::Static_Draw );
	IndexBuffer = VAO::CreateBuffer( VAO::Index_Buffer, NumberIndices, &Array_Indices[ 0 ], VAO::Static_Draw );

	VAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( le::BSPVertex ), ( void* ) ( offsetof( le::BSPVertex, Position ) ) );
	VAO::SetVertexAttribPointer( VERT_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof( le::BSPVertex ), ( void* ) ( offsetof( le::BSPVertex, Normal ) ) );
	VAO::SetVertexAttribPointer( VERT_TEXCOORD_DIFFUSE, 2, GL_FLOAT, GL_FALSE, sizeof( le::BSPVertex ), ( void* ) ( offsetof( le::BSPVertex, TextureCoord ) ) );
	VAO::SetVertexAttribPointer( VERT_TEXCOORD_LIGHTMAP, 2, GL_FLOAT, GL_FALSE, sizeof( le::BSPVertex ), ( void* ) ( offsetof( le::BSPVertex, LightmapCoord ) ) );

	VAO::UnbindVAO();
	VAO::UnbindBuffer( VAO::Vertex_Buffer );
	VAO::UnbindBuffer( VAO::Index_Buffer );

	// *****************************************
	// Освобождаем выделенную память

	delete[] Array_Vertexes;
	delete[] Array_Indices;
	delete[] Array_Faces;
	delete[] Array_Textures;

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
	if ( !ArrayPlanes.empty() )
	{
		for ( auto It = ArrayPlanes.begin(); It != ArrayPlanes.end(); It++ )
			for ( size_t i = 0; i < It->second.size(); i++ )
				delete ArrayPlanes[ It->first ][ i ];

		ArrayPlanes.clear();
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
	for ( size_t i = 0; i < ArrayEntitys.size(); i++ )
		if ( ArrayEntitys[ i ].GetNameEntity() == NameEntity )
			return &ArrayEntitys[ i ];

	return NULL;
}

//-------------------------------------------------------------------------//

map<GLuint, vector<le::Plane*> >& le::Level::GetAllPlanes()
{
	return ArrayPlanes;
}

//-------------------------------------------------------------------------//

vector<le::Entity>& le::Level::GetAllEntitys()
{
	return ArrayEntitys;
}

//-------------------------------------------------------------------------//

