#include <System\Logger.h>
#include <System\ResourcesManager.h>
#include <Graphics\Camera.h>
#include <Graphics\Scene.h>
#include <Graphics\LightManager.h>
#include "..\Level.h"

//-------------------------------------------------------------------------//

le::Level::Level( System& System ) :
	Scene( NULL )
{
	Skybox = new le::Skybox();
	Skybox->SetSizeSkybox( 500 );
}

//-------------------------------------------------------------------------//

le::Level::~Level()
{
	if ( !Brushes.empty() )
		for ( size_t i = 0; i < Brushes.size(); i++ )
			delete Brushes[ i ];

	delete Skybox;
}

//-------------------------------------------------------------------------//

bool le::Level::LoadLevel( const string& Route )
{
	Logger::Log( Logger::Info, "Loading Level [" + Route + "]" );

	TiXmlDocument Level;

	if ( !Level.LoadFile( Route.c_str() ) )
	{
		Logger::Log( Logger::Error, "Level [" + Route + "] Not Found Or Not Currect Format" );
		return false;
	}

	TiXmlElement* Map;
	Map = Level.FirstChildElement( "Map" );

	if ( Map == NULL )
	{
		Logger::Log( Logger::Error, "Not Correct Format In Level [" + Route + "]. Not Found Tag \"Map\"" );
		return false;
	}

	//====== ЗАГРУЖАЕМ НАСТРОЙКИ КАРТЫ ======//

	TiXmlElement* Settings;
	Settings = Map->FirstChildElement( "Settings" );

	if ( Settings != NULL )
	{
		TiXmlElement *NameMap, *DescriptionMap, *SkyBoxName;
		NameMap = Settings->FirstChildElement( "NameMap" );
		DescriptionMap = Settings->FirstChildElement( "DescriptionMap" );
		SkyBoxName = Settings->FirstChildElement( "SkyBoxName" );

		this->NameMap = NameMap->Attribute( "Value" );
		this->DescriptionMap = DescriptionMap->Attribute( "Value" );
		this->SkyBoxName = SkyBoxName->Attribute( "Value" );

		if ( !this->SkyBoxName.empty() )
			Skybox->LoadSkybox( this->SkyBoxName );
		else
			Logger::Log( Logger::Warning, "On The Level Missing Skybox" );
	}

	//====== ЗАГРУЖАЕМ ТЕКСТУРЫ КОТОРЫЕ ИСП. НА КАРТЕ ======//

	TiXmlElement *Textures, *Texture;
	map<string, GLuint> mTextures;
	Textures = Map->FirstChildElement( "Textures" );

	if ( Textures != NULL )
	{
		string Name, Route;
		Texture = Textures->FirstChildElement( "Texture" );

		while ( Texture )
		{
			Name = Texture->Attribute( "Name" );
			Route = Texture->Attribute( "Route" );

			ResourcesManager::LoadGlTexture( Name, Route );
			mTextures[ Name ] = ResourcesManager::GetGlTexture( Name );

			Texture = Texture->NextSiblingElement();
		}
	}
	else
		Logger::Log( Logger::Warning, "In Level [" + Route + "]. Not Found Tag \"Textures\"" );

	//====== ЗАГРУЖАЕМ БРАШИ ======//

	TiXmlElement* Brushes;
	Brushes = Map->FirstChildElement( "Brushes" );

	if ( Brushes == NULL )
	{
		Logger::Log( Logger::Error, "Not Correct Format In Level [" + Route + "]. Not Found Tag \"Brushes\"" );
		return false;
	}

	// ****************************
	// Загружаем твердые браши
	// ****************************

	TiXmlElement* Solid;
	Solid = Brushes->FirstChildElement( "Solid" );

	if ( Solid != NULL )
	{
		TiXmlElement* Brush;
		string str_TypeBrush, str_TextureName, str_Temp;
		glm::vec3 Position;
		glm::vec3 TempVector3;
		glm::vec2 TempVector2;
		vector<string> NameLightmaps;
		vector<glm::vec3> Vertexs, Normals;
		vector<glm::vec2> TexCoords, TexCoords_Lightmap;

		Brush = Solid->FirstChildElement( "Brush" );

		while ( Brush )
		{
			TiXmlElement *Type, *TextureName, *PositionBrush;

			Type = Brush->FirstChildElement( "Type" );
			TextureName = Brush->FirstChildElement( "TextureName" );
			PositionBrush = Brush->FirstChildElement( "Position" );

			str_TypeBrush = Type->Attribute( "Value" );
			str_TextureName = TextureName->Attribute( "Value" );

			Position.x = NUMBER_TO_FLOAT( atof( PositionBrush->Attribute( "X" ) ) );
			Position.y = NUMBER_TO_FLOAT( atof( PositionBrush->Attribute( "Y" ) ) );
			Position.z = NUMBER_TO_FLOAT( atof( PositionBrush->Attribute( "Z" ) ) );

			// ****************************
			// Загружаем позиции вершин
			// ****************************

			TiXmlElement *PositionVertex, *Vertex;
			PositionVertex = Brush->FirstChildElement( "PositionVertex" );
			Vertex = PositionVertex->FirstChildElement( "Vertex" );

			while ( Vertex )
			{
				TempVector3.x = NUMBER_TO_FLOAT( atof( Vertex->Attribute( "X" ) ) );
				TempVector3.y = NUMBER_TO_FLOAT( atof( Vertex->Attribute( "Y" ) ) );
				TempVector3.z = NUMBER_TO_FLOAT( atof( Vertex->Attribute( "Z" ) ) );

				Vertexs.push_back( TempVector3 );
				Vertex = Vertex->NextSiblingElement();
			}

			// ****************************
			// Загружаем нормали
			// ****************************

			TiXmlElement *xml_Normals, *Point;
			xml_Normals = Brush->FirstChildElement( "Normals" );
			Point = xml_Normals->FirstChildElement( "Point" );

			while ( Point )
			{
				TempVector3.x = NUMBER_TO_FLOAT( atof( Point->Attribute( "X" ) ) );
				TempVector3.y = NUMBER_TO_FLOAT( atof( Point->Attribute( "Y" ) ) );
				TempVector3.z = NUMBER_TO_FLOAT( atof( Point->Attribute( "Z" ) ) );

				Normals.push_back( TempVector3 );
				Point = Point->NextSiblingElement();
			}

			// ****************************
			// Загружаем текстурные координаты
			// ****************************

			TiXmlElement *TextureCoords;
			TextureCoords = Brush->FirstChildElement( "TextureCoords" );
			Point = TextureCoords->FirstChildElement( "Point" );

			while ( Point )
			{
				TempVector2.x = NUMBER_TO_FLOAT( atof( Point->Attribute( "X" ) ) );
				TempVector2.y = NUMBER_TO_FLOAT( atof( Point->Attribute( "Y" ) ) );

				TexCoords.push_back( TempVector2 );
				Point = Point->NextSiblingElement();
			}

			// ****************************
			// Загружаем текстурные координаты карты освещения
			// ****************************

			TiXmlElement *TextureCoords_LightMap;
			TextureCoords_LightMap = Brush->FirstChildElement( "TextureCoords_LightMap" );
			Point = TextureCoords_LightMap->FirstChildElement( "Point" );

			while ( Point )
			{
				TempVector2.x = NUMBER_TO_FLOAT( atof( Point->Attribute( "X" ) ) );
				TempVector2.y = NUMBER_TO_FLOAT( atof( Point->Attribute( "Y" ) ) );

				TexCoords_Lightmap.push_back( TempVector2 );
				Point = Point->NextSiblingElement();
			}


			// ****************************
			// Запоминаем названия лайтмапы для каждого треугольника
			// ****************************

			TiXmlElement *LightMaps;
			LightMaps = Brush->FirstChildElement( "LightMaps" );
			TiXmlElement *Triangle = LightMaps->FirstChildElement( "Triangle" );

			while ( Triangle )
			{
				NameLightmaps.push_back( Triangle->Attribute( "Route" ) );
				Triangle = Triangle->NextSiblingElement();
			}

			le::Brush* LevelBrush = new le::Brush();
			LevelBrush->CreateBrush( Brush::Cube, Position, mTextures[ str_TextureName ], Vertexs, Normals, TexCoords, TexCoords_Lightmap, NameLightmaps );

			this->Brushes.push_back( LevelBrush );

			Vertexs.clear();
			Normals.clear();
			TexCoords.clear();
			TexCoords_Lightmap.clear();
			NameLightmaps.clear();
			Brush = Brush->NextSiblingElement();
		}
	}

	//====== ЗАГРУЖАЕМ ЭНТИТИ-ОБЪЕКТЫ ======//

	TiXmlElement *xml_Entitys;
	xml_Entitys = Map->FirstChildElement( "Entitys" );

	if ( xml_Entitys == NULL )
	{
		Logger::Log( Logger::Error, "Not Correct Format In Level [" + Route + "]. Not Found Tag \"Entitys\"" );
		return false;
	}

	TiXmlElement *xml_Entity;
	xml_Entity = xml_Entitys->FirstChildElement( "Entity" );

	while ( xml_Entity )
	{
		Entity Entity( *xml_Entity );
		Entitys.push_back( Entity );

		xml_Entity = xml_Entity->NextSiblingElement();
	}

	Logger::Log( Logger::Info, "Level [" + Route + "] Loaded" );
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
	if ( !Brushes.empty() )
		for ( size_t i = 0; i < Brushes.size(); i++ )
			delete Brushes[ i ];
}

//-------------------------------------------------------------------------//

le::Skybox& le::Level::GetSkybox()
{
	return *Skybox;
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

vector<le::Brush*>& le::Level::GetAllBrushes()
{
	return Brushes;
}

//-------------------------------------------------------------------------//

vector<le::Entity>& le::Level::GetAllEntitys()
{
	return Entitys;
}

//-------------------------------------------------------------------------//

