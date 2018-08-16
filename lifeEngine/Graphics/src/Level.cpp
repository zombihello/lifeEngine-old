#include <System\Logger.h>
#include <System\ResourcesManager.h>
#include <Graphics\Camera.h>
#include <Graphics\Scene.h>
#include <Graphics\LightManager.h>
#include "..\Level.h"

#define FACE_POLYGON 1

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
		for ( size_t i = 0; i < ArrayPlanes.size(); i++ )
			delete ArrayPlanes[ i ];

		ArrayPlanes.clear();
	}

	for ( int i = 0; i < ArrayLightmaps.size(); i++ )
		glDeleteTextures( 1, &ArrayLightmaps[ i ] );

	if ( VertexBuffer != 0 )
	{
		VAO::DeleteVAO( &ArrayBuffer );
		VAO::DeleteBuffer( &VertexBuffer );
		VAO::DeleteBuffer( &IndexBuffer );
	}

	ArrayLightmaps.clear();
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

	// TODO: [zombiHello] Переделать массивы с Си стиля на vector'ы
	int						NumberVertexes = 0;
	int						NumberFaces = 0;
	int						NumberIndices = 0;
	int						NumberTextures = 0;
	int						NumberLightmaps = 0;
	unsigned int*			Array_Indices = NULL;

	BSPHeader				Header = { 0 };
	BSPLump					Lumps[ MaxLumps ] = { 0 };
	BSPVertex*				Array_Vertexes = NULL;
	BSPFace*				Array_Faces = NULL;
	BSPTexture*				Array_Textures = NULL;
	BSPLightmap*			Array_Lightmaps = NULL;
	vector<GLuint>			GLTextures;
	vector<BSPVertex>		VAO_Vertexes;
	vector<unsigned int>	VAO_Indices;

	// *************************************
	// Загружаем заголовок и куски файла

	File.read( ( char* ) &Header, sizeof( BSPHeader ) );
	File.read( ( char* ) &Lumps, MaxLumps * sizeof( BSPLump ) );

	if ( string( Header.StrID, 4 ) != "IBSP" || Header.Version != 46 )
	{
		Logger::Log( Logger::Error, "Not Supported Format BSP [Type: " + string( Header.StrID, 4 ) + " Version: " + to_string( Header.Version ) + "]" );
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

	// Выделяем память для информации о карте освещения
	NumberLightmaps = Lumps[ Lightmaps ].Length / sizeof( BSPLightmap );
	Array_Lightmaps = new BSPLightmap[ NumberLightmaps ];

	// Выделяем память для веток BSP дерева
	ArrayNodes.resize( Lumps[ Nodes ].Length / sizeof( BSPNode ) );

	// Выделяем память для листьев BSP дерева
	ArrayLeafs.resize( Lumps[ Leafs ].Length / sizeof( BSPLeaf ) );

	// Выделяем память для массива индексов фейсов в листе
	ArrayLeafsFaces.resize( Lumps[ LeafFaces ].Length / sizeof( int ) );

	// Выделяем память для секущих плоскостей BSP дерева
	ArrayBSPPlanes.resize( Lumps[ Planes ].Length / sizeof( BSPPlane ) );

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

	// Если карт освещения нет, то загружаем черную текстуру
	if ( NumberLightmaps == 0 )
	{
		CreateLightmapTexture( NULL, 1, 1 );

		for ( int IdFace = 0; IdFace < NumberFaces; IdFace++ )
			Array_Faces[ IdFace ].LightmapID = 0;
	}
	else
	{
		// Смещаемся на участок в файле, в котором хранится информация о карте освещения
		File.seekg( Lumps[ Lightmaps ].Offset, ios::beg );

		for ( int i = 0; i < NumberLightmaps; i++ )
		{
			// Считываем карту освещения
			File.read( ( char* ) &Array_Lightmaps[ i ], sizeof( BSPLightmap ) );
			CreateLightmapTexture( ( unsigned char * ) Array_Lightmaps[ i ].ImageBits, 128, 128 );
		}
	}

	// Смещаемся на участок в файле, в котором хранится информация о ветках BSP дерева
	File.seekg( Lumps[ Nodes ].Offset, ios::beg );
	File.read( ( char* ) &ArrayNodes[ 0 ], ArrayNodes.size() * sizeof( BSPNode ) );

	// Смещаемся на участок в файле, в котором хранится информация о листьях BSP дерева
	File.seekg( Lumps[ Leafs ].Offset, ios::beg );
	File.read( ( char* ) &ArrayLeafs[ 0 ], ArrayLeafs.size() * sizeof( BSPLeaf ) );

	// *****************************************
	// Меняем ось Z и Y местами

	for ( size_t i = 0; i < ArrayLeafs.size(); i++ )
	{
		BSPLeaf* Leaf = &ArrayLeafs[ i ];

		int Temp = Leaf->Min.y;
		Leaf->Min.y = Leaf->Min.z;
		Leaf->Min.z = -Temp;

		Temp = Leaf->Max.y;
		Leaf->Max.y = Leaf->Max.z;
		Leaf->Max.z = -Temp;
	}

	// Смещаемся на участок в файле, в котором хранится информация о ветках BSP дерева
	File.seekg( Lumps[ LeafFaces ].Offset, ios::beg );
	File.read( ( char* ) &ArrayLeafsFaces[ 0 ], ArrayLeafsFaces.size() * sizeof( int ) );

	// Смещаемся на участок в файле, в котором хранится информация о секущих плоскостях BSP дерева
	File.seekg( Lumps[ Planes ].Offset, ios::beg );
	File.read( ( char* ) &ArrayBSPPlanes[ 0 ], ArrayBSPPlanes.size() * sizeof( BSPPlane ) );

	// *****************************************
	// Меняем ось Z и Y местами

	for ( size_t i = 0; i < ArrayBSPPlanes.size(); i++ )
	{
		float Temp = ArrayBSPPlanes[ i ].Normal.y;
		ArrayBSPPlanes[ i ].Normal.y = ArrayBSPPlanes[ i ].Normal.z;
		ArrayBSPPlanes[ i ].Normal.z = -Temp;
	}

	// Смещаемся на участок в файле, в котором хранится информация о видимой геометрии
	File.seekg( Lumps[ VisData ].Offset, ios::beg );

	if ( Lumps[ VisData ].Length )
	{
		File.read( ( char* ) &Сlusters.NumOfClusters, sizeof( int ) );
		File.read( ( char* ) &Сlusters.BytesPerCluster, sizeof( int ) );

		int Size = Сlusters.NumOfClusters * Сlusters.BytesPerCluster;
		Сlusters.Bitsets = new byte[ Size ];

		File.read( ( char* ) &Сlusters.Bitsets[ 0 ], Size * sizeof( byte ) );
	}
	else
		Сlusters.Bitsets = NULL;

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
		Plane*		Plane = new le::Plane();
		BSPFace*	Face = &Array_Faces[ i ];
		int			StartIndex = VAO_Indices.size();

		// *****************************************
		// Конвертируем вершины и индексы для VAO

		for ( int IdIndex = Face->StartIndex, Count = 0; Count < Face->NumOfIndices; Count++, IdIndex++ )
		{
			BSPVertex* TempVertex = &Array_Vertexes[ Face->StartVertIndex + Array_Indices[ IdIndex ] ];

			bool IsFind = false;
			for ( int Id = 0; Id < VAO_Vertexes.size(); Id++ )
				if ( *TempVertex == VAO_Vertexes[ Id ] )
				{
					VAO_Indices.push_back( Id );
					IsFind = true;
					break;
				}

			if ( !IsFind )
			{
				VAO_Indices.push_back( VAO_Vertexes.size() );
				VAO_Vertexes.push_back( *TempVertex );
			}
		}

		Plane->Type = Face->Type;
		Plane->StartIndex = StartIndex;
		Plane->NumberIndices = VAO_Indices.size() - StartIndex;
		Plane->Texture = GLTextures[ Face->TextureID ];

		if ( Face->LightmapID >= 0 )
			Plane->Lightmap = ArrayLightmaps[ Face->LightmapID ];
		else
			Plane->Lightmap = 0;

		ArrayPlanes.push_back( Plane );
	}

	// *****************************************
	// Создаем VAO

	ArrayBuffer = VAO::CreateVAO();
	VAO::BindVAO( ArrayBuffer );

	VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, VAO_Vertexes, VAO::Static_Draw );
	IndexBuffer = VAO::CreateBuffer( VAO::Index_Buffer, VAO_Indices, VAO::Static_Draw );

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
	delete[] Array_Lightmaps;

	FacesDrawn.Resize( NumberFaces );
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
		for ( size_t i = 0; i < ArrayPlanes.size(); i++ )
			delete ArrayPlanes[ i ];

		ArrayPlanes.clear();
	}

	for ( int i = 0; i < ArrayLightmaps.size(); i++ )
		glDeleteTextures( 1, &ArrayLightmaps[ i ] );

	if ( VertexBuffer != 0 )
	{
		VAO::DeleteVAO( &ArrayBuffer );
		VAO::DeleteBuffer( &VertexBuffer );
		VAO::DeleteBuffer( &IndexBuffer );
	}

	ArrayLightmaps.clear();
}

//-------------------------------------------------------------------------//

void le::Level::CalculateVisablePlanes( Camera& Camera )
{
	VisablePlanes.clear();
	FacesDrawn.ClearAll();

	int LeafIndex = FindLeaf( Camera.GetPosition() );
	int Cluster = ArrayLeafs[ LeafIndex ].Cluster;
	int FaceIndex = 0;

	BSPLeaf* Leaf;
	Plane* Plane;

	for ( int i = 0; i < ArrayLeafs.size(); i++ )
	{
		Leaf = &ArrayLeafs[ i ];

		if( !IsClusterVisible( Cluster, Leaf->Cluster ) || !Camera.GetFrustum().IsVisible( Leaf->Min, Leaf->Max ) ) 
			continue;

		for ( int j = 0; j < Leaf->NumOfLeafFaces; j++ )
		{
			FaceIndex = ArrayLeafsFaces[ Leaf->LeafFace + j ];

			if ( !FacesDrawn.On( FaceIndex ) )
			{
				Plane = ArrayPlanes[ FaceIndex ];

				if ( Plane->Type != FACE_POLYGON ) continue;

				FacesDrawn.Set( FaceIndex );
				VisablePlanes[ Plane->Texture ].push_back( Plane );
			}
		}
	}
}

//-------------------------------------------------------------------------//

void le::Level::CalculateVisablePlanes( const glm::vec3& Position, Frustum& Frustum )
{
	VisablePlanes.clear();
	FacesDrawn.ClearAll();

	int LeafIndex = FindLeaf( Position );
	int Cluster = ArrayLeafs[ LeafIndex ].Cluster;
	int FaceIndex = 0;

	BSPLeaf* Leaf;
	Plane* Plane;

	for ( int i = 0; i < ArrayLeafs.size(); i++ )
	{
		Leaf = &ArrayLeafs[ i ];

		if ( !IsClusterVisible( Cluster, Leaf->Cluster ) || !Frustum.IsVisible( Leaf->Min, Leaf->Max ) ) 
			continue;

		for ( int j = 0; j < Leaf->NumOfLeafFaces; j++ )
		{
			FaceIndex = ArrayLeafsFaces[ Leaf->LeafFace + j ];

			if ( !FacesDrawn.On( FaceIndex ) )
			{
				Plane = ArrayPlanes[ FaceIndex ];

				if ( Plane->Type != FACE_POLYGON ) continue;

				FacesDrawn.Set( FaceIndex );
				VisablePlanes[ Plane->Texture ].push_back( Plane );
			}
		}
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

map<GLuint, vector<le::Plane*> >& le::Level::GetVisablePlanes()
{
	return VisablePlanes;
}

//-------------------------------------------------------------------------//

vector<le::Entity>& le::Level::GetAllEntitys()
{
	return ArrayEntitys;
}

//-------------------------------------------------------------------------//

void le::Level::ChangeGamma( byte* ImageBits, int Size, float Factor )
{
	// Проходим по каждому пикселю в карте освещения
	for ( int i = 0; i < Size / 3; i++, ImageBits += 3 )
	{
		float scale = 1.0f, temp = 0.0f;
		float r = 0, g = 0, b = 0;

		// Извлекаем текущий цвет пикселя
		r = ( float ) ImageBits[ 0 ];
		g = ( float ) ImageBits[ 1 ];
		b = ( float ) ImageBits[ 2 ];

		// Умножаем коэффициент на значение RGB, сохраняя при этом соотношение 255
		r = r * Factor / 255.0f;
		g = g * Factor / 255.0f;
		b = b * Factor / 255.0f;

		// Проверяем, прошли ли значения выше самого высокого значения
		if ( r > 1.0f && ( temp = ( 1.0f / r ) ) < scale ) scale = temp;
		if ( g > 1.0f && ( temp = ( 1.0f / g ) ) < scale ) scale = temp;
		if ( b > 1.0f && ( temp = ( 1.0f / b ) ) < scale ) scale = temp;

		// Получаем масштаб для этого пикселя и умножаем его на значения пикселей
		scale *= 255.0f;
		r *= scale;	g *= scale;	b *= scale;

		// Запоминаем новый цвет пикселя
		ImageBits[ 0 ] = ( byte ) r;
		ImageBits[ 1 ] = ( byte ) g;
		ImageBits[ 2 ] = ( byte ) b;
	}
}

//-------------------------------------------------------------------------//

void le::Level::CreateLightmapTexture( byte* ImageBits, int Width, int Height )
{
	GLuint Texture = 0;

	glGenTextures( 1, &Texture );
	glBindTexture( GL_TEXTURE_2D, Texture );

	if ( ImageBits )
	{
		ChangeGamma( ImageBits, Width * Height * 3, 10 );
		gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, Width, Height, GL_RGB, GL_UNSIGNED_BYTE, ImageBits );
	}
	else
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	ArrayLightmaps.push_back( Texture );
}

//-------------------------------------------------------------------------//

int le::Level::FindLeaf( const glm::vec3 & Position )
{
	int			i = 0;
	float		Distance = 0.f;

	while ( i >= 0 )
	{
		const BSPNode&  Node = ArrayNodes[ i ];
		const BSPPlane& Plane = ArrayBSPPlanes[ Node.Plane ];

		Distance = Plane.Normal.x * Position.x + Plane.Normal.y * Position.y + Plane.Normal.z * Position.z - Plane.Distance;

		if ( Distance >= 0 )
			i = Node.Front;
		else
			i = Node.Back;
	}

	return ~i;
}

//-------------------------------------------------------------------------//

bool le::Level::IsClusterVisible( int CurrentCluster, int TestCluster )
{
	if ( !Сlusters.Bitsets || CurrentCluster < 0 ) return 1;

	byte VisSet = Сlusters.Bitsets[ ( CurrentCluster * Сlusters.BytesPerCluster ) + ( TestCluster / 8 ) ];
	int Result = VisSet & ( 1 << ( ( TestCluster ) & 7 ) );

	return Result;
}

//-------------------------------------------------------------------------//

bool le::BSPVertex::operator==( BSPVertex& Vertex )
{
	return
		Position == Vertex.Position &&
		TextureCoord == Vertex.TextureCoord &&
		LightmapCoord == Vertex.LightmapCoord &&
		Normal == Vertex.Normal &&
		Color[ 0 ] == Vertex.Color[ 0 ] &&
		Color[ 1 ] == Vertex.Color[ 1 ] &&
		Color[ 2 ] == Vertex.Color[ 2 ] &&
		Color[ 3 ] == Vertex.Color[ 3 ];
}

//-------------------------------------------------------------------------//
