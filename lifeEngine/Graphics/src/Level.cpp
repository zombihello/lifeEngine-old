#include <System\VAO.h>
#include "..\Level.h"

//-------------------------------------------------------------------------//

le::Level::Level( System& System ) :
	Scene( NULL )
{
	Skybox = new le::Skybox( System );
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

		Skybox->LoadSkybox( this->SkyBoxName );
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
		string str_TypeBrush, str_TextureName;
		glm::vec3 Position;
		glm::vec3 TempVector3;
		glm::vec2 TempVector2;
		vector<glm::vec3> Vertexs, Normals;
		vector<glm::vec2> TexCoords;

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

			Level::Brush* LevelBrush = new Level::Brush();
			LevelBrush->CreateBrush( Brush::Cube, Position, mTextures[ str_TextureName ], Vertexs, Normals, TexCoords );

			this->Brushes.push_back( LevelBrush );

			Vertexs.clear();
			Normals.clear();
			TexCoords.clear();
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

le::Level::Entity* le::Level::GetEntity( const string& NameEntity )
{
	for ( size_t i = 0; i < Entitys.size(); i++ )
		if ( Entitys[ i ].GetNameEntity() == NameEntity )
			return &Entitys[ i ];

	return NULL;
}

//-------------------------------------------------------------------------//

vector<le::Level::Brush*>& le::Level::GetAllBrushes()
{
	return Brushes;
}

//-------------------------------------------------------------------------//

vector<le::Level::Entity>& le::Level::GetAllEntitys()
{
	return Entitys;
}

//-------------------------------------------------------------------------//


///////////////////////////////////
//			КЛАСС БРАШ
//////////////////////////////////

//-------------------------------------------------------------------------//

le::Level::Brush::Brush() :
	VertexBuffer( 0 ),
	IndexBuffer( 0 ),
	ArrayBuffer( 0 )
{}

//-------------------------------------------------------------------------//

le::Level::Brush::~Brush()
{
	if ( VertexBuffer != 0 )
		VAO::DeleteBuffer( &VertexBuffer );

	if ( IndexBuffer != 0 )
		VAO::DeleteBuffer( &IndexBuffer );

	if ( ArrayBuffer != 0 )
		VAO::DeleteVAO( &ArrayBuffer );
}

//-------------------------------------------------------------------------//

void le::Level::Brush::CreateBrush( const PrimitivesType& TypeBrush, const glm::vec3& Position, const GLuint& Texture, const vector<glm::vec3>& Vertex, const vector<glm::vec3>& Normals, const vector<glm::vec2>& TextureCoords )
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

	Brush::BrushVertex TempVertex;

	for ( size_t i = 0; i < TmpIdVertex.size(); i++ )
	{
		TempVertex.Position = Vertex[ TmpIdVertex[ i ] ];
		TempVertex.Normal = Normals[ i ];
		TempVertex.TextureCoord = TextureCoords[ i ];

		bool IsFind = false;

		for ( size_t j = 0; j < BrushVertex.size(); j++ )
			if ( TempVertex == BrushVertex[ j ] )
			{
				IdVertex.push_back( j );
				IsFind = true;
				break;
			}

		if ( !IsFind )
		{
			IdVertex.push_back( BrushVertex.size() );
			BrushVertex.push_back( TempVertex );
		}
	}

	Scene::InfoMesh InfoMesh;

	ArrayBuffer = VAO::CreateVAO();
	VAO::BindVAO( ArrayBuffer );

	VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, BrushVertex, VAO::Static_Draw );
	IndexBuffer = VAO::CreateBuffer( VAO::Index_Buffer, IdVertex, VAO::Static_Draw );

	VAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( Brush::BrushVertex ), ( void* ) ( offsetof( Brush::BrushVertex, Position ) ) );
	VAO::SetVertexAttribPointer( VERT_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof( Brush::BrushVertex ), ( void* ) ( offsetof( Brush::BrushVertex, Normal ) ) );
	VAO::SetVertexAttribPointer( VERT_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof( Brush::BrushVertex ), ( void* ) ( offsetof( Brush::BrushVertex, TextureCoord ) ) );

	VAO::UnbindVAO();
	VAO::UnbindBuffer( VAO::Vertex_Buffer );
	VAO::UnbindBuffer( VAO::Index_Buffer );

	InfoMesh.CountIndexs = NUMBER_TO_INT( IdVertex.size() );
	InfoMesh.VertexArray = ArrayBuffer;
	InfoMesh.BoundingBox = &BoundingBox;
	InfoMesh.Position = &this->Position;

	RenderMesh[ Texture ] = InfoMesh;
}

//-------------------------------------------------------------------------//

map<GLuint, le::Scene::InfoMesh>& le::Level::Brush::GetRenderMesh()
{
	return RenderMesh;
}

//-------------------------------------------------------------------------//

bool le::Level::Brush::BrushVertex::operator==( BrushVertex& BrushVertex )
{
	return Position == BrushVertex.Position && Normal == BrushVertex.Normal && TextureCoord == BrushVertex.TextureCoord;
}

//-------------------------------------------------------------------------//

///////////////////////////////////
//			КЛАСС ЭНТИТИ
//////////////////////////////////

//-------------------------------------------------------------------------//

le::Level::Entity::Entity( TiXmlElement& ElementEntity )
{
	// ***************************************** //
	// Загружаем название энтити

	NameEntity = ElementEntity.Attribute( "Name" );

	// ***************************************** //
	// Загружаем позицию энтити в мире

	TiXmlElement* position;
	position = ElementEntity.FirstChildElement( "Position" );

	Position.x = NUMBER_TO_FLOAT( atof( position->Attribute( "X" ) ) );
	Position.y = NUMBER_TO_FLOAT( atof( position->Attribute( "Y" ) ) );
	Position.z = NUMBER_TO_FLOAT( atof( position->Attribute( "Z" ) ) );

	// ***************************************** //
	// Загружаем значения у энтити

	TiXmlElement* Value;
	Value = ElementEntity.FirstChildElement( "Value" );

	while ( Value )
	{
		Values[ Value->Attribute( "Name" ) ] = Value->Attribute( "Value" );
		Value = Value->NextSiblingElement();
	}
}

//-------------------------------------------------------------------------//

string& le::Level::Entity::GetNameEntity()
{
	return NameEntity;
}

//-------------------------------------------------------------------------//

string le::Level::Entity::GetValueString( const string& NameValue )
{
	if ( Values.find( NameValue ) != Values.end() )
		return Values[ NameValue ];

	return "";
}

//-------------------------------------------------------------------------//

int le::Level::Entity::GetValueInt( const string& NameValue )
{
	if ( Values.find( NameValue ) != Values.end() )
		return atoi( Values[ NameValue ].c_str() );

	return -1;
}

//-------------------------------------------------------------------------//

float le::Level::Entity::GetValueFloat( const string& NameValue )
{
	if ( Values.find( NameValue ) != Values.end() )
		return NUMBER_TO_FLOAT( atof( Values[ NameValue ].c_str() ) );

	return -1.f;
}

//-------------------------------------------------------------------------//

vector<string> le::Level::Entity::GetVelueVectorString( const string& NameValue )
{
	vector<string> Vector;

	if ( Values.find( NameValue ) != Values.end() )
	{
		string TempString;
		stringstream StringStream( Values[ NameValue ] );

		while ( !StringStream.eof() )
		{
			StringStream >> TempString;
			Vector.push_back( TempString );
			TempString.clear();
		}
	}

	return Vector;
}

//-------------------------------------------------------------------------//

vector<int> le::Level::Entity::GetVelueVectorInt( const string& NameValue )
{
	vector<int> Vector;

	if ( Values.find( NameValue ) != Values.end() )
	{
		string TempString;
		stringstream StringStream( Values[ NameValue ] );

		while ( !StringStream.eof() )
		{
			StringStream >> TempString;
			Vector.push_back( atoi( TempString.c_str() ) );
			TempString.clear();
		}
	}

	return Vector;
}

//-------------------------------------------------------------------------//

vector<float> le::Level::Entity::GetVelueVectorFloat( const string& NameValue )
{
	vector<float> Vector;

	if ( Values.find( NameValue ) != Values.end() )
	{
		string TempString;
		stringstream StringStream( Values[ NameValue ] );

		while ( !StringStream.eof() )
		{
			StringStream >> TempString;
			Vector.push_back( NUMBER_TO_FLOAT( atof( TempString.c_str() ) ) );
			TempString.clear();
		}
	}

	return Vector;
}

//-------------------------------------------------------------------------//

glm::vec3& le::Level::Entity::GetPosition()
{
	return Position;
}

//-------------------------------------------------------------------------//
