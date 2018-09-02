#include <System\Logger.h>
#include <System\ResourcesManager.h>

#include <Graphics\Camera.h>
#include <Graphics\Scene.h>
#include <Graphics\Light\LightManager.h>
#include <Graphics\Models\Model.h>
#include <Graphics\Level\Level.h>

#include <Graphics\Level\Entityes\Func_Door.h>
#include <Graphics\LEvel\Entityes\Func_Rotating.h>
#include <Graphics\Level\Entityes\Info_Player_Start.h>
#include <Graphics\Level\Entityes\Worldspawn.h>

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
		for ( size_t IdPlane = 0; IdPlane < ArrayPlanes.size(); IdPlane++ )
			delete ArrayPlanes[ IdPlane ];

		ArrayPlanes.clear();
	}

	for ( size_t IdLightmaps = 0; IdLightmaps < ArrayLightmaps.size(); IdLightmaps++ )
		glDeleteTextures( 1, &ArrayLightmaps[ IdLightmaps ] );

	if ( VertexBuffer != 0 )
	{
		VAO::DeleteVAO( &ArrayBuffer );
		VAO::DeleteBuffer( &VertexBuffer );
		VAO::DeleteBuffer( &IndexBuffer );
	}

	for ( size_t IdEntity = 0; IdEntity < ArrayEntitys.size(); IdEntity++ )
		delete ArrayEntitys[ IdEntity ];

	delete Skybox;
}

//-------------------------------------------------------------------------//

bool le::Level::LoadLevel( const string& Route )
{
	Logger::Log( Logger::Info, "Loading Level [" + Route + "]" );

	// *************************************
	// Открываем файл карты

	ifstream File( Route, ios::binary );

	if ( !File.is_open() )
	{
		Logger::Log( Logger::Error, "BSP File [" + Route + "] Not Found" );
		return false;
	}

	BSPHeader				Header = { 0 };
	BSPLump					Lumps[ MaxLumps ] = { 0 };
	BSPEntities				BSPEntities;

	vector<BSPVertex>		Array_Vertexes;
	vector<BSPFace>			Array_Faces;
	vector<BSPTexture>		Array_Textures;
	vector<BSPLightmap>		Array_Lightmaps;
	vector<GLuint>			GLTextures;
	vector<int>				Buffer_ArrayLeafsFaces;
	vector<unsigned int>	Array_Indices;

	// *************************************
	// Загружаем заголовок и куски файла

	File.read( ( char* ) &Header, sizeof( BSPHeader ) );
	File.read( ( char* ) &Lumps, MaxLumps * sizeof( BSPLump ) );

	if ( string( Header.StrID, 4 ) != "IBSP" || Header.Version != 46 )
	{
		Logger::Log( Logger::Error, "Not Supported Format BSP [Type: " + string( Header.StrID, 4 ) + " Version: " + to_string( Header.Version ) + "]" );
		return false;
	}

	// *************************************
	// Вычисляем необходимые размеры массивов прд данные

	BSPEntities.EntitiesData = new char[ Lumps[ Entities ].Length ];				// Выделяем память под информацию энтити-объектов

	Array_Vertexes.resize( Lumps[ Vertices ].Length / sizeof( BSPVertex ) );		// Массив вершин
	Array_Faces.resize( Lumps[ Faces ].Length / sizeof( BSPFace ) );				// Массив фейсов
	Array_Indices.resize( Lumps[ Indices ].Length / sizeof( unsigned int ) );		// Массив индексов
	Array_Textures.resize( Lumps[ Textures ].Length / sizeof( BSPTexture ) );		// Массив текстур
	Array_Lightmaps.resize( Lumps[ Lightmaps ].Length / sizeof( BSPLightmap ) );	// Массив карт освещения
	ArrayNodes.resize( Lumps[ Nodes ].Length / sizeof( BSPNode ) );					// Массив веток BSP дерева
	ArrayLeafs.resize( Lumps[ Leafs ].Length / sizeof( BSPLeaf ) );					// Массив листьев BSP дерева
	Buffer_ArrayLeafsFaces.resize( Lumps[ LeafFaces ].Length / sizeof( int ) );		// Массив индексов фейсов в листе BSP дерева
	ArrayBSPPlanes.resize( Lumps[ Planes ].Length / sizeof( BSPPlane ) );			// Массив секущих плоскостей
	ArrayModels.resize( Lumps[ Models ].Length / sizeof( BSPModel ) );				// Массив моделей (движущиеся браши)

	// *************************************
	// Считываем информацию энтити-объектов

	File.seekg( Lumps[ Entities ].Offset, ios::beg );
	File.read( ( char * ) &BSPEntities.EntitiesData[ 0 ], Lumps[ Entities ].Length );

	// *************************************
	// Считываем вершины

	File.seekg( Lumps[ Vertices ].Offset, ios::beg );

	for ( size_t IdVertex = 0; IdVertex < Array_Vertexes.size(); IdVertex++ )
	{
		File.read( ( char* ) &Array_Vertexes[ IdVertex ], sizeof( BSPVertex ) );
		BSPVertex* Vertex = &Array_Vertexes[ IdVertex ];

		// Меняем значения Y и Z, и отрицаем новый Z, чтобы Y был вверх.
		float Temp = Vertex->Position.y;
		Vertex->Position.y = Vertex->Position.z;
		Vertex->Position.z = -Temp;

		Temp = Vertex->Normal.y;
		Vertex->Normal.y = Vertex->Normal.z;
		Vertex->Normal.z = -Temp;

		Vertex->TextureCoord.y = -Vertex->TextureCoord.y;
	}

	// *************************************
	// Считываем индексы вершин

	File.seekg( Lumps[ Indices ].Offset, ios::beg );
	File.read( ( char* ) &Array_Indices[ 0 ], Array_Indices.size() * sizeof( unsigned int ) );

	// *************************************
	// Считываем информацию о фейсах

	File.seekg( Lumps[ Faces ].Offset, ios::beg );
	File.read( ( char* ) &Array_Faces[ 0 ], Array_Faces.size() * sizeof( BSPFace ) );

	// *************************************
	// Считываем информацию о текстурах

	File.seekg( Lumps[ Textures ].Offset, ios::beg );
	File.read( ( char* ) &Array_Textures[ 0 ], Array_Textures.size() * sizeof( BSPTexture ) );

	// *************************************
	// Считываем информацию о моделях уровня 

	File.seekg( Lumps[ Models ].Offset, ios::beg );

	for ( size_t IdModel = 0; IdModel < ArrayModels.size(); IdModel++ )
	{
		File.read( ( char * ) &ArrayModels[ IdModel ], sizeof( BSPModel ) );
		BSPModel* Model = &ArrayModels[ IdModel ];

		float Temp = Model->Min.y;
		Model->Min.y = Model->Min.z;
		Model->Min.z = -Temp;

		Temp = Model->Max.y;
		Model->Max.y = Model->Max.z;
		Model->Max.z = -Temp;
	}


	// *************************************
	// Считываем карту освещения

	if ( Array_Lightmaps.size() == 0 )
	{
		CreateLightmapTexture( NULL, 1, 1 );

		for ( size_t IdFace = 0; IdFace < Array_Faces.size(); IdFace++ )
			Array_Faces[ IdFace ].LightmapID = 0;
	}
	else
	{
		File.seekg( Lumps[ Lightmaps ].Offset, ios::beg );

		for ( size_t IdLightmap = 0; IdLightmap < Array_Lightmaps.size(); IdLightmap++ )
		{
			File.read( ( char* ) &Array_Lightmaps[ IdLightmap ], sizeof( BSPLightmap ) );
			CreateLightmapTexture( ( unsigned char * ) Array_Lightmaps[ IdLightmap ].ImageBits, 128, 128 );
		}
	}

	// *************************************
	// Считываем информацию о ветках BSP дерева

	File.seekg( Lumps[ Nodes ].Offset, ios::beg );
	File.read( ( char* ) &ArrayNodes[ 0 ], ArrayNodes.size() * sizeof( BSPNode ) );

	// *************************************
	// Считываем информацию о листьях BSP дерева

	File.seekg( Lumps[ Leafs ].Offset, ios::beg );
	File.read( ( char* ) &ArrayLeafs[ 0 ], ArrayLeafs.size() * sizeof( BSPLeaf ) );

	// Меняем ось Z и Y местами
	for ( size_t IdLeaf = 0; IdLeaf < ArrayLeafs.size(); IdLeaf++ )
	{
		BSPLeaf* Leaf = &ArrayLeafs[ IdLeaf ];

		int Temp = Leaf->Min.y;
		Leaf->Min.y = Leaf->Min.z;
		Leaf->Min.z = -Temp;

		Temp = Leaf->Max.y;
		Leaf->Max.y = Leaf->Max.z;
		Leaf->Max.z = -Temp;
	}

	// *************************************
	// Считываем информацию о ветках BSP дерева

	File.seekg( Lumps[ LeafFaces ].Offset, ios::beg );
	File.read( ( char* ) &Buffer_ArrayLeafsFaces[ 0 ], Buffer_ArrayLeafsFaces.size() * sizeof( int ) );

	// *************************************
	// Убираем индексы фейсов относящиеся к движ. части уровня

	int		FaceStart = ArrayModels[ 0 ].StartFaceIndex;
	int		FaceEnd = ArrayModels[ 0 ].StartFaceIndex + ArrayModels[ 0 ].NumOfFaces - 1;

	for ( size_t IdLeaf = 0; IdLeaf < ArrayLeafs.size(); IdLeaf++ )
	{
		BSPLeaf* Leaf = &ArrayLeafs[ IdLeaf ];
		int LeafFace = ArrayLeafsFaces.size();

		for ( int j = 0; j < Leaf->NumOfLeafFaces; j++ )
		{
			int FaceIndex = Buffer_ArrayLeafsFaces[ Leaf->LeafFace + j ];

			if ( FaceIndex >= FaceStart && FaceIndex <= FaceEnd )
				ArrayLeafsFaces.push_back( FaceIndex );
		}

		Leaf->LeafFace = LeafFace;
		Leaf->NumOfLeafFaces = ArrayLeafsFaces.size() - LeafFace;
	}

	// *************************************
	// Считываем информацию о секущих плоскостях BSP дерева

	File.seekg( Lumps[ Planes ].Offset, ios::beg );
	File.read( ( char* ) &ArrayBSPPlanes[ 0 ], ArrayBSPPlanes.size() * sizeof( BSPPlane ) );

	// Меняем ось Z и Y местами
	for ( size_t IdPlane = 0; IdPlane < ArrayBSPPlanes.size(); IdPlane++ )
	{
		BSPPlane* Plane = &ArrayBSPPlanes[ IdPlane ];

		float Temp = Plane->Normal.y;
		Plane->Normal.y = Plane->Normal.z;
		Plane->Normal.z = -Temp;
	}

	// *************************************
	// Считываем информацию о видимой геометрии

	if ( Lumps[ VisData ].Length )
	{
		File.seekg( Lumps[ VisData ].Offset, ios::beg );
		File.read( ( char* ) &Сlusters.NumOfClusters, sizeof( int ) );
		File.read( ( char* ) &Сlusters.BytesPerCluster, sizeof( int ) );

		int Size = Сlusters.NumOfClusters * Сlusters.BytesPerCluster;
		Сlusters.Bitsets = new byte[ Size ];

		File.read( ( char* ) &Сlusters.Bitsets[ 0 ], Size * sizeof( byte ) );
	}
	else
		Сlusters.Bitsets = NULL;

	// *************************************
	// Загружаем все текстуры

	for ( size_t IdTexture = 0; IdTexture < Array_Textures.size(); IdTexture++ )
	{
		// Определяем формат текстуры
		string RouteToTexture = "../" + string( Array_Textures[ IdTexture ].StrName );
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
		ResourcesManager::LoadGlTexture( Array_Textures[ IdTexture ].StrName, RouteToTexture );
		GLTextures.push_back( ResourcesManager::GetGlTexture( Array_Textures[ IdTexture ].StrName ) );
	}

	// *****************************************
	// Инициализируем плоскости

	for ( size_t IdFace = 0; IdFace < Array_Faces.size(); IdFace++ )
	{
		Plane*		Plane = new le::Plane();
		BSPFace*	Face = &Array_Faces[ IdFace ];

		Plane->Type = Face->Type;
		Plane->StartVertex = Face->StartVertIndex;
		Plane->StartIndex = Face->StartIndex;
		Plane->NumberIndices = Face->NumOfIndices;
		Plane->Texture = GLTextures[ Face->TextureID ];

		if ( Face->LightmapID >= 0 )
			Plane->Lightmap = ArrayLightmaps[ Face->LightmapID ];
		else
			Plane->Lightmap = 0;

		ArrayPlanes.push_back( Plane );
	}

	// *************************************
	// Инициализируем энтити-объекты

	EntityParser EntityParser( BSPEntities, Lumps[ Entities ].Length );

	for ( auto ItEntityes = EntityParser.Entityes.begin(); ItEntityes != EntityParser.Entityes.end(); ItEntityes++ )
	{
		if ( ItEntityes->ClassName == "func_door" )
		{
			Func_Door*		Func_Door = new le::Func_Door( ItEntityes->Values );
			int				IdModel = Func_Door->GetIdModel();

			if ( IdModel == -1 || IdModel >= NUMBER_TO_INT( ArrayModels.size() ) )
			{
				delete Func_Door;
				continue;
			}

			BSPModel*		Model = &ArrayModels[ IdModel ];

			Func_Door->SetModel( *Model );
			ArrayModelEntitys[ Model ] = Func_Door;
			ArrayEntitys.push_back( Func_Door );
		}
		else if ( ItEntityes->ClassName == "func_rotating" )
		{
			Func_Rotating*		Func_Rotating = new le::Func_Rotating( ItEntityes->Values );
			int					IdModel = Func_Rotating->GetIdModel();

			if ( IdModel == -1 || IdModel >= NUMBER_TO_INT( ArrayModels.size() ) )
			{
				delete Func_Rotating;
				continue;
			}

			BSPModel*		Model = &ArrayModels[ IdModel ];

			Func_Rotating->SetModel( *Model );
			ArrayModelEntitys[ Model ] = Func_Rotating;
			ArrayEntitys.push_back( Func_Rotating );
		}
		else if ( ItEntityes->ClassName == "worldspawn" )
		{
			ArrayEntitys.push_back( new Worldspawn( ItEntityes->Values ) );
		}
		else if ( ItEntityes->ClassName == "info_player_start" )
		{
			ArrayEntitys.push_back( new Info_Player_Start( ItEntityes->Values ) );
		}
		else
		{
			NoInitEntity.push_back( *ItEntityes );
		}
	}

	// *****************************************
	// Создаем VAO

	ArrayBuffer = VAO::CreateVAO();
	VAO::BindVAO( ArrayBuffer );

	VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, Array_Vertexes, VAO::Static_Draw );
	IndexBuffer = VAO::CreateBuffer( VAO::Index_Buffer, Array_Indices, VAO::Static_Draw );

	VAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( le::BSPVertex ), ( void* ) ( offsetof( le::BSPVertex, Position ) ) );
	VAO::SetVertexAttribPointer( VERT_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof( le::BSPVertex ), ( void* ) ( offsetof( le::BSPVertex, Normal ) ) );
	VAO::SetVertexAttribPointer( VERT_TEXCOORD_DIFFUSE, 2, GL_FLOAT, GL_FALSE, sizeof( le::BSPVertex ), ( void* ) ( offsetof( le::BSPVertex, TextureCoord ) ) );
	VAO::SetVertexAttribPointer( VERT_TEXCOORD_LIGHTMAP, 2, GL_FLOAT, GL_FALSE, sizeof( le::BSPVertex ), ( void* ) ( offsetof( le::BSPVertex, LightmapCoord ) ) );

	VAO::UnbindVAO();
	VAO::UnbindBuffer( VAO::Vertex_Buffer );
	VAO::UnbindBuffer( VAO::Index_Buffer );

	// *****************************************
	// Считываем данные окружающей среды

	Worldspawn* Entity_Worldspawn = GetEntity<Worldspawn>( "worldspawn" );

	if ( Entity_Worldspawn )
	{
		if ( !Entity_Worldspawn->Skybox.empty() )
			//TODO: [zombiHello] Данный путь временный, сделать возможность задать где храниться скайбокс + сделать свой формат хранения скайбокса
			Skybox->LoadSkybox( "../textures/skybox/" + Entity_Worldspawn->Skybox + ".jpg" );
	}

	FacesDrawn.Resize( Array_Faces.size() );
	Logger::Log( Logger::Info, "Level [" + Route + "] Loaded" );
	return true;
}

//-------------------------------------------------------------------------//

void le::Level::AddToScene( le::Scene& Scene )
{
	this->Scene = &Scene;
	Scene.AddLevel( this );
}

//-------------------------------------------------------------------------//

void le::Level::RemoveFromScene()
{
	if ( Scene )
		Scene->RemoveLevel();
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
		for ( size_t IdPlanes = 0; IdPlanes < ArrayPlanes.size(); IdPlanes++ )
			delete ArrayPlanes[ IdPlanes ];

		ArrayPlanes.clear();
	}

	for ( size_t IdLightmaps = 0; IdLightmaps < ArrayLightmaps.size(); IdLightmaps++ )
		glDeleteTextures( 1, &ArrayLightmaps[ IdLightmaps ] );

	if ( VertexBuffer != 0 )
	{
		VAO::DeleteVAO( &ArrayBuffer );
		VAO::DeleteBuffer( &VertexBuffer );
		VAO::DeleteBuffer( &IndexBuffer );
	}

	for ( size_t IdEntity = 0; IdEntity < ArrayEntitys.size(); IdEntity++ )
		delete ArrayEntitys[ IdEntity ];

	ArrayLightmaps.clear();
	ArrayEntitys.clear();
}

//-------------------------------------------------------------------------//

void le::Level::CalculateVisablePlanes( Camera& Camera )
{
	VisablePlanes.clear();
	VisableModels.clear();
	FacesDrawn.ClearAll();

	int									LeafIndex = FindLeaf( Camera.GetPosition() );
	int									FaceIndex = 0;
	CameraCluster = ArrayLeafs[ LeafIndex ].Cluster;

	BSPLeaf*							Leaf;
	Plane*								Plane;
	Frustum*							Frustum = &Camera.GetFrustum();
	map<GLuint, int>					ArrayTextures;

	// *****************************************
	// Считаем видимые плоскости статической геометрии

	for ( size_t IdLeaf = 0; IdLeaf < ArrayLeafs.size(); IdLeaf++ )
	{
		Leaf = &ArrayLeafs[ IdLeaf ];

		if ( !IsClusterVisible( CameraCluster, Leaf->Cluster ) || !Frustum->IsVisible( Leaf->Min, Leaf->Max ) )
			continue;

		for ( int IdLeafFace = 0; IdLeafFace < Leaf->NumOfLeafFaces; IdLeafFace++ )
		{
			FaceIndex = ArrayLeafsFaces[ Leaf->LeafFace + IdLeafFace ];

			if ( !FacesDrawn.On( FaceIndex ) )
			{
				Plane = ArrayPlanes[ FaceIndex ];
				FacesDrawn.Set( FaceIndex );

				if ( ArrayTextures.find( Plane->Texture ) != ArrayTextures.end() )
					VisablePlanes[ ArrayTextures[ Plane->Texture ] ].RenderPlanes.push_back( Plane );
				else
				{
					InfoBSPPolygon			InfoRender;
					InfoRender.Texture = Plane->Texture;
					InfoRender.RenderPlanes.push_back( Plane );

					VisablePlanes.push_back( InfoRender );
					ArrayTextures[ Plane->Texture ] = VisablePlanes.size() - 1;
				}
			}
		}
	}

	// *****************************************
	// Считаем видимые плоскости динамической 
	// геометрии (двери, лифты и т.д)

	ArrayTextures.clear();

	for ( auto ItModel = ArrayModelEntitys.begin(); ItModel != ArrayModelEntitys.end(); ItModel++ )
	{
		BSPModel* Model = ItModel->first;

		LeafIndex = FindLeaf( ( Model->Max + Model->Min ) / 2.f );
		BSPLeaf* Leaf = &ArrayLeafs[ LeafIndex ];
		int Cluster = Leaf->Cluster;

		if ( !IsClusterVisible( Cluster, CameraCluster ) || !Frustum->IsVisible( Model->Min, Model->Max ) )
			continue;

		size_t EndFace = Model->StartFaceIndex + Model->NumOfFaces;
		ItModel->second->Update();

		for ( size_t IdFace = Model->StartFaceIndex; IdFace < EndFace; IdFace++ )
			if ( !FacesDrawn.On( IdFace ) )
			{
				Plane = ArrayPlanes[ IdFace ];
				FacesDrawn.Set( IdFace );

				if ( ArrayTextures.find( Plane->Texture ) != ArrayTextures.end() )
					VisableModels[ ArrayTextures[ Plane->Texture ] ].RenderPlanes[ &ItModel->second->Transformation ].push_back( Plane );
				else
				{
					InfoBSPModel			InfoRender;
					InfoRender.Texture = Plane->Texture;
					InfoRender.RenderPlanes[ &ItModel->second->Transformation ].push_back( Plane );

					VisableModels.push_back( InfoRender );
					ArrayTextures[ Plane->Texture ] = VisableModels.size() - 1;
				}
			}
	}
}

//-------------------------------------------------------------------------//

void le::Level::CalculateVisablePlanes( const glm::vec3& Position, Frustum& Frustum )
{
	VisablePlanes.clear();
	VisableModels.clear();
	FacesDrawn.ClearAll();

	int									LeafIndex = FindLeaf( Position );
	int									FaceIndex = 0;
	CameraCluster = ArrayLeafs[ LeafIndex ].Cluster;

	BSPLeaf*							Leaf;
	Plane*								Plane;
	map<GLuint, int>					ArrayTextures;

	// *****************************************
	// Считаем видимые плоскости статической геометрии

	for ( size_t IdLeaf = 0; IdLeaf < ArrayLeafs.size(); IdLeaf++ )
	{
		Leaf = &ArrayLeafs[ IdLeaf ];

		if ( !IsClusterVisible( CameraCluster, Leaf->Cluster ) || !Frustum.IsVisible( Leaf->Min, Leaf->Max ) )
			continue;

		for ( int IdLeafFace = 0; IdLeafFace < Leaf->NumOfLeafFaces; IdLeafFace++ )
		{
			FaceIndex = ArrayLeafsFaces[ Leaf->LeafFace + IdLeafFace ];

			if ( !FacesDrawn.On( FaceIndex ) )
			{
				Plane = ArrayPlanes[ FaceIndex ];
				FacesDrawn.Set( FaceIndex );

				if ( ArrayTextures.find( Plane->Texture ) != ArrayTextures.end() )
					VisablePlanes[ ArrayTextures[ Plane->Texture ] ].RenderPlanes.push_back( Plane );
				else
				{
					InfoBSPPolygon			InfoRender;
					InfoRender.Texture = Plane->Texture;
					InfoRender.RenderPlanes.push_back( Plane );

					VisablePlanes.push_back( InfoRender );
					ArrayTextures[ Plane->Texture ] = VisablePlanes.size() - 1;
				}
			}
		}
	}

	// *****************************************
	// Считаем видимые плоскости динамической 
	// геометрии (двери, лифты и т.д)

	ArrayTextures.clear();

	for ( auto ItModel = ArrayModelEntitys.begin(); ItModel != ArrayModelEntitys.end(); ItModel++ )
	{
		BSPModel* Model = ItModel->first;

		LeafIndex = FindLeaf( ( Model->Max + Model->Min ) / 2.f );
		BSPLeaf* Leaf = &ArrayLeafs[ LeafIndex ];
		int Cluster = Leaf->Cluster;

		if ( !IsClusterVisible( Cluster, CameraCluster ) || !Frustum.IsVisible( Model->Min, Model->Max ) )
			continue;

		size_t EndFace = Model->StartFaceIndex + Model->NumOfFaces;
		ItModel->second->Update();

		for ( size_t IdFace = Model->StartFaceIndex; IdFace < EndFace; IdFace++ )
			if ( !FacesDrawn.On( IdFace ) )
			{
				Plane = ArrayPlanes[ IdFace ];
				FacesDrawn.Set( IdFace );

				if ( ArrayTextures.find( Plane->Texture ) != ArrayTextures.end() )
					VisableModels[ ArrayTextures[ Plane->Texture ] ].RenderPlanes[ &ItModel->second->Transformation ].push_back( Plane );
				else
				{
					InfoBSPModel			InfoRender;
					InfoRender.Texture = Plane->Texture;
					InfoRender.RenderPlanes[ &ItModel->second->Transformation ].push_back( Plane );

					VisableModels.push_back( InfoRender );
					ArrayTextures[ Plane->Texture ] = VisableModels.size() - 1;
				}
			}
	}
}

//-------------------------------------------------------------------------//

void le::Level::CalculateVisableModels( vector<Model*>& Models, Frustum& Frustum )
{
	for ( size_t IdModel = 0; IdModel < Models.size(); IdModel++ )
	{
		Model* Model = Models[ IdModel ];

		int LeafIndex = FindLeaf( ( Model->GetMaxVertex() + Model->GetMinVertex() ) / 2.f );
		BSPLeaf* Leaf = &ArrayLeafs[ LeafIndex ];
		int Cluster = Leaf->Cluster;

		Model->SetRender( IsClusterVisible( Cluster, CameraCluster ) && Frustum.IsVisible( Model->GetMinVertex(), Model->GetMaxVertex() ) );
	}
}

//-------------------------------------------------------------------------//

bool le::Level::CalculateVisableLights( vector<PointLight>& Lights, Frustum& Frustum )
{
	bool IsVisibleLight = false;

	for ( size_t IdLight = 0; IdLight < Lights.size(); IdLight++ )
	{
		PointLight* Light = &Lights[ IdLight ];

		Light->IsVisible = false;
		int LeafIndex = FindLeaf( Light->Position );
		BSPLeaf* Leaf = &ArrayLeafs[ LeafIndex ];
		int Cluster = Leaf->Cluster;

		Light->IsVisible = IsClusterVisible( Cluster, CameraCluster ) && ( Frustum.IsVisible( Leaf->Min, Leaf->Max ) || Frustum.IsVisible( Light->Position, Light->Radius ) );

		if ( !IsVisibleLight && Light->IsVisible )
			IsVisibleLight = true;
	}

	return IsVisibleLight;
}

//-------------------------------------------------------------------------//

bool le::Level::CalculateVisableLights( vector<SpotLight>& Lights, Frustum& Frustum )
{
	bool IsVisibleLight = false;

	for ( size_t IdLight = 0; IdLight < Lights.size(); IdLight++ )
	{
		SpotLight* Light = &Lights[ IdLight ];

		Light->IsVisible = false;
		int LeafIndex = FindLeaf( Light->Position );
		BSPLeaf* Leaf = &ArrayLeafs[ LeafIndex ];
		int Cluster = Leaf->Cluster;

		Light->IsVisible = IsClusterVisible( Cluster, CameraCluster ) && ( Frustum.IsVisible( Leaf->Min, Leaf->Max ) || Frustum.IsVisible( Light->Position, Light->Radius ) );

		if ( !IsVisibleLight && Light->IsVisible )
			IsVisibleLight = true;
	}

	return IsVisibleLight;
}

//-------------------------------------------------------------------------//

bool le::Level::CalculateVisableModel( Model& Model, Frustum& Frustum )
{
	bool IsRender = false;
	int LeafIndex = FindLeaf( ( Model.GetMaxVertex() + Model.GetMinVertex() ) / 2.f );
	BSPLeaf* Leaf = &ArrayLeafs[ LeafIndex ];
	int Cluster = Leaf->Cluster;

	IsRender = IsClusterVisible( Cluster, CameraCluster ) && Frustum.IsVisible( Model.GetMinVertex(), Model.GetMaxVertex() );
	Model.SetRender( IsRender );

	return IsRender;
}

//-------------------------------------------------------------------------//

bool le::Level::CalculateVisableLight( PointLight& Light, Frustum& Frustum )
{
	Light.IsVisible = false;
	int LeafIndex = FindLeaf( Light.Position );
	BSPLeaf* Leaf = &ArrayLeafs[ LeafIndex ];
	int Cluster = Leaf->Cluster;

	Light.IsVisible = IsClusterVisible( Cluster, CameraCluster ) && ( Frustum.IsVisible( Leaf->Min, Leaf->Max ) || Frustum.IsVisible( Light.Position, Light.Radius ) );

	return Light.IsVisible;
}

//-------------------------------------------------------------------------//

bool le::Level::CalculateVisableLight( SpotLight& Light, Frustum& Frustum )
{
	Light.IsVisible = false;
	int LeafIndex = FindLeaf( Light.Position );
	BSPLeaf* Leaf = &ArrayLeafs[ LeafIndex ];
	int Cluster = Leaf->Cluster;

	Light.IsVisible = IsClusterVisible( Cluster, CameraCluster ) && ( Frustum.IsVisible( Leaf->Min, Leaf->Max ) || Frustum.IsVisible( Light.Position, Light.Radius ) );

	return Light.IsVisible;
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

vector< le::InfoBSPPolygon >& le::Level::GetVisablePlanes()
{
	return VisablePlanes;
}

//-------------------------------------------------------------------------//

vector<le::InfoBSPModel>& le::Level::GetVisableModels()
{
	return VisableModels;
}

//-------------------------------------------------------------------------//

vector<le::Entity>&	le::Level::GetNoInitEntity()
{
	return NoInitEntity;
}

//-------------------------------------------------------------------------//

vector<le::BaseEntity*>& le::Level::GetAllEntitys()
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
		//ChangeGamma( ImageBits, Width * Height * 3, 10 );
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
	int			Index = 0;
	float		Distance = 0.f;

	while ( Index >= 0 )
	{
		const BSPNode&  Node = ArrayNodes[ Index ];
		const BSPPlane& Plane = ArrayBSPPlanes[ Node.Plane ];

		Distance = glm::dot( Plane.Normal, Position ) - Plane.Distance;

		if ( Distance >= 0 )
			Index = Node.Front;
		else
			Index = Node.Back;
	}

	return -Index - 1;
}

//-------------------------------------------------------------------------//

inline bool le::Level::IsClusterVisible( int CurrentCluster, int TestCluster )
{
	if ( !Сlusters.Bitsets || CurrentCluster < 0 )
		return true;

	byte VisSet = Сlusters.Bitsets[ CurrentCluster * Сlusters.BytesPerCluster + ( TestCluster >> 3 ) ];

	if ( !( VisSet & ( 1 << ( TestCluster & 7 ) ) ) )
		return false;

	return true;
}

//-------------------------------------------------------------------------//