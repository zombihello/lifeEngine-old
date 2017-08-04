#include "..\..\3D\Level3D.h"
#include "..\..\..\System\MaterialManager.h"

//-------------------------------------------------------------------------//

le::Level3D::Level3D( le::System& System, le::Scene3D& Scene, le::Physic3D& Physic )
{
	this->System = &System;
	this->Physic = &Physic;
	this->Scene = &Scene;
}

//-------------------------------------------------------------------------//

le::Level3D::Level3D( le::System& System, le::Physic3D& Physic )
{
	this->System = &System;
	this->Physic = &Physic;
	Scene = NULL;
}

//-------------------------------------------------------------------------//

le::Level3D::~Level3D()
{
	ClearLevel();
}

//-------------------------------------------------------------------------//

bool le::Level3D::LoadLevel( string sRoute )
{
	map<string, GLuint> mTextures;
	TiXmlDocument LMAP;

	if ( !LMAP.LoadFile( sRoute.c_str() ) )
		return false;

	// Работаем с контейнером Map
	TiXmlElement *Map;
	Map = LMAP.FirstChildElement( "Map" );

	// -----MAP SETTINGS----- //

	// Работаем с контейнером Settings
	TiXmlElement *Settings;
	Settings = Map->FirstChildElement( "Settings" );

	// Работаем с контейнером NameMap
	TiXmlElement *NameMap;
	NameMap = Settings->FirstChildElement( "NameMap" );

	if ( NameMap->Attribute( "Value" ) != NULL )
		sNameMap = NameMap->Attribute( "Value" );

	// Работаем с контейнером DescriptionMap
	TiXmlElement *DescriptionMap;
	DescriptionMap = Settings->FirstChildElement( "DescriptionMap" );

	if ( DescriptionMap->Attribute( "Value" ) != NULL )
		sDescriptionMap = DescriptionMap->Attribute( "Value" );

	// Работаем с контейнером SkyBoxName
	TiXmlElement *SkyBoxName;
	SkyBoxName = Settings->FirstChildElement( "SkyBoxName" );

	if ( SkyBoxName->Attribute( "Value" ) != NULL )
		sSkyBoxName = SkyBoxName->Attribute( "Value" );

	// -----TEXTURES----- //

	// Работаем с контейнером Textures
	TiXmlElement *Textures;
	TiXmlElement *Texture;
	Textures = Map->FirstChildElement( "Textures" );

	if ( Textures != NULL )
	{
		Texture = Textures->FirstChildElement( "Texture" );

		while ( Texture )
		{
			string name = Texture->Attribute( "Name" );
			string route = Texture->Attribute( "Route" );

			MaterialManager::LoadGLTexture( name, route );
			mTextures[name] = MaterialManager::GetGLTexture( name );
			Texture = Texture->NextSiblingElement();
		}
	}

	// -----BRUSHES----- //

	// Работаем с контейнером Brushes
	TiXmlElement *Brushes;
	Brushes = Map->FirstChildElement( "Brushes" );

	// -----BRUSHES | SOLID----- //

	// Работаем с контейнером Solid
	TiXmlElement *Solid;
	Solid = Brushes->FirstChildElement( "Solid" );

	if ( Solid != NULL )
	{
		// Работаем с контейнером Brush
		TiXmlElement *Brush;
		Brush = Solid->FirstChildElement( "Brush" );

		while ( Brush )
		{
			string typeBrush;
			string textureName;

			vector<glm::vec3> vertex;
			vector<glm::vec3> normals;
			vector<glm::vec2> texCoord;

			// Работаем с контейнером Type
			TiXmlElement *Type;
			Type = Brush->FirstChildElement( "Type" );

			if ( Type->Attribute( "Value" ) != NULL )
				typeBrush = Type->Attribute( "Value" );

			// Работаем с контейнером TextureName
			TiXmlElement *TextureName;
			TextureName = Brush->FirstChildElement( "TextureName" );

			if ( TextureName->Attribute( "Value" ) != NULL )
				textureName = TextureName->Attribute( "Value" );

			// Работаем с контейнером PositionVertex
			TiXmlElement *PositionVertex;
			TiXmlElement *Vertex;
			PositionVertex = Brush->FirstChildElement( "PositionVertex" );
			Vertex = PositionVertex->FirstChildElement( "Vertex" );

			while ( Vertex )
			{
				glm::vec3 tmpVertex;

				tmpVertex.x = atof( Vertex->Attribute( "X" ) );
				tmpVertex.y = atof( Vertex->Attribute( "Y" ) );
				tmpVertex.z = atof( Vertex->Attribute( "Z" ) );

				vertex.push_back( tmpVertex );
				Vertex = Vertex->NextSiblingElement();
			}

			// Работаем с контейнером Normals
			TiXmlElement *Normals;
			TiXmlElement *Point;
			Normals = Brush->FirstChildElement( "Normals" );
			Point = Normals->FirstChildElement( "Point" );

			while ( Point )
			{
				glm::vec3 tmpPoint;

				tmpPoint.x = atof( Point->Attribute( "X" ) );
				tmpPoint.y = atof( Point->Attribute( "Y" ) );
				tmpPoint.z = atof( Point->Attribute( "Z" ) );

				normals.push_back( tmpPoint );
				Point = Point->NextSiblingElement();
			}

			// Работаем с контейнером TextureCoords
			TiXmlElement *TextureCoords;
			TextureCoords = Brush->FirstChildElement( "TextureCoords" );
			Point = TextureCoords->FirstChildElement( "Point" );

			while ( Point )
			{
				glm::vec2 tmpPoint;

				tmpPoint.x = atof( Point->Attribute( "X" ) );
				tmpPoint.y = atof( Point->Attribute( "Y" ) );

				texCoord.push_back( tmpPoint );
				Point = Point->NextSiblingElement();
			}


			le::Brush* tmpBrush;

			if ( Scene != NULL )
				tmpBrush = new le::Brush( *System, *Scene, *Physic );
			else
				tmpBrush = new le::Brush( *System, *Physic );

			tmpBrush->CreateBrush( Brush::PrimitivesType::Cube, mTextures[textureName], vertex, normals, texCoord );
			vBrushes.push_back( tmpBrush );

			Brush = Brush->NextSiblingElement();
		}
	}

	// -----ENTITYS----- //

	// Работаем с контейнером Entitys
	TiXmlElement *Entitys;
	Entitys = Map->FirstChildElement( "Entitys" );

	// -----ENTITY----- //

	// Работаем с контейнером Entity
	TiXmlElement *entity;
	entity = Entitys->FirstChildElement( "Entity" );

	while ( entity )
	{
		Entity Entity( *entity );
		vEntitys.push_back( Entity );

		entity = entity->NextSiblingElement();
	}

	return true;
}

//-------------------------------------------------------------------------//

void le::Level3D::ClearLevel()
{
	sNameMap = "";
	sDescriptionMap = "";
	sSkyBoxName = "";

	for ( int i = 0; i < vBrushes.size(); i++ )
		delete vBrushes[i];

	vBrushes.clear();
	vEntitys.clear();
}

//-------------------------------------------------------------------------//

void le::Level3D::UpdateLevel( Shader* Shader )
{
	if ( Scene != NULL )
	for ( int i = 0; i < vBrushes.size(); i++ )
		vBrushes[i]->UpdateBrush( Shader );
}

//-------------------------------------------------------------------------//

le::Entity* le::Level3D::GetEntity( string NameEntity )
{
	for ( int i = 0; i < vEntitys.size(); i++ )
	if ( vEntitys[i].GetNameEntity() == NameEntity )
		return &vEntitys[i];

	return NULL;
}

//-------------------------------------------------------------------------//

vector<le::Entity>& le::Level3D::GetAllEntitys()
{
	return vEntitys;
}

//-------------------------------------------------------------------------//

vector<le::Brush*>& le::Level3D::GetAllBrushes()
{
	return vBrushes;
}

//-------------------------------------------------------------------------//

void le::Level3D::AddToScene( le::Scene3D& Scene )
{
	this->Scene = &Scene;

	for ( int i = 0; i < vBrushes.size(); i++ )
		vBrushes[i]->AddToScene( Scene );
}

//-------------------------------------------------------------------------//